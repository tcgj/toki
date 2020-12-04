#include "bvh.hpp"

#include "core/ray.hpp"
#include "region/primitive.hpp"

namespace TK {
    // Construct a BVH based on the provided construction type
    BVH::BVH(const std::vector<std::shared_ptr<Primitive>>& primitiveList, Strategy type) {
        int64_t pCount = primitiveList.size();
        if (pCount == 0)
            return;
        // Initialise array to maximum number of nodes required
        // which is 2n - 1 nodes assuming 1 primitive per leaf node
        nodes = std::make_unique<Node[]>(2 * pCount - 1);

        std::vector<PrimitiveUnit> pSet;
        for (int64_t i = 0; i < pCount; ++i) {
            pSet.emplace_back(i, primitiveList[i]->worldBoundingBox());
        }

        switch (type) {
            case BINNED_SAH:
                buildSAH(0, 0, pCount, pSet);
                break;
            default:
                break;
        }

        std::vector<std::shared_ptr<Primitive>> pSorted;
        for (int64_t i = 0; i < pCount; ++i) {
            pSorted.push_back(primitiveList[pSet[i].index]);
        }
        primitives.swap(pSorted);
        pSet.clear();
    }

    AABBf BVH::worldBoundingBox() const {
        return nodes != nullptr ? nodes[0].bb : AABBf();
    }

    // Traverse the BVH until intersection with a primitive
    bool BVH::hasIntersect(const Ray& r) const {
        return intersectNode(r, 0, nullptr);
    }

    // Traverse the BVH to find the closest surface interaction
    bool BVH::intersect(const Ray& r, SurfaceInteraction* interaction) const {
        return intersectNode(r, 0, interaction);
    }

    // Start traversal from particular node
    bool BVH::intersectNode(const Ray& r, int64_t nodeIndex, SurfaceInteraction* interaction) const {
        // Pre-calculation for aabb intersection test
        Vec3f invD(1.0 / r.d.x, 1.0 / r.d.y, 1.0 / r.d.z);
        int dirNegative[3] = { invD.x < 0, invD.y < 0, invD.z < 0 };

        bool hit = false;
        int sp = 0;
        int64_t stack[64] = { 0 };
        int64_t curr;
        while (sp >= 0) {
            curr = stack[sp--];
            if (!nodes[curr].bb.hasIntersect(r, invD, dirNegative)) {
                continue;
            }
            // Intersected bounding box, check if leaf/interior
            if (nodes[curr].count > 0) {
                if (interaction != nullptr) {
                    for (int64_t i = 0; i < nodes[curr].count; ++i) {
                        if (primitives[nodes[curr].offset + i]->intersect(r, interaction))
                            hit = true;
                    }
                } else {
                    for (int64_t i = 0; i < nodes[curr].count; ++i) {
                        if (primitives[nodes[curr].offset + i]->hasIntersect(r))
                            return true;
                    }
                }
            } else {
                if (dirNegative[nodes[curr].axis]) {
                    stack[++sp] = nodes[curr].left;
                    stack[++sp] = nodes[curr].right;
                } else {
                    stack[++sp] = nodes[curr].right;
                    stack[++sp] = nodes[curr].left;
                }
            }
        }
        return hit;
    }

    /* ----- SAH-based Binned BVH Construction ----- */
    constexpr int binCount = 16;
    constexpr int maxPCount = 2;

    struct SAHBin {
        int64_t count = 0;
        AABBf bb;
    };

    // Construct a SAH-based BVH
    void BVH::buildSAH(int64_t nodeIndex, int64_t start, int64_t end, std::vector<PrimitiveUnit>& pSet) {
        int64_t count = end - start;

        /* Make leaf node since no subdivision required */
        if (count == 1) {
            nodes[nodeIndex].makeLeaf(pSet[start].bb, count, start);
            return;
        }

        /* Compute bounding boxes for current node */
        AABBf bb;
        AABBf centroidBB;
        for (int64_t i = start; i < end; ++i) {
            bb = bbUnion(bb, pSet[i].bb);
            centroidBB = bbUnion(centroidBB, pSet[i].centroid);
        }
        // Get widest axis to subdivide (works reasonably well)
        int axis = centroidBB.maxExtent();
        // Centroids are almost at the same point, make leaf node
        // since subdivision unlikely to improve efficiency
        if (centroidBB.maxPt[axis] - centroidBB.minPt[axis] < TK_EPSILON) {
            nodes[nodeIndex].makeLeaf(bb, count, start);
            return;
        }

        /* Init bins for partitioning tests */
        SAHBin bins[binCount];
        tkFloat axisWidth = centroidBB.diagonal()[axis];
        tkFloat k1 = binCount * (1 - TK_EPSILON) / axisWidth;
        tkFloat k0 = centroidBB.minPt[axis];
        for (int64_t i = start; i < end; ++i) {
            // Map primitive to bin index
            int binIndex = k1 * (pSet[i].centroid[axis] - k0);
            bins[binIndex].count++;
            bins[binIndex].bb = bbUnion(bins[binIndex].bb, pSet[i].bb);
        }

        /* Compute SAH data at each n-1 partition */
        SAHBin lPart[binCount - 1];
        SAHBin rPart[binCount - 1];
        lPart[0] = bins[0];
        rPart[binCount - 2] = bins[binCount - 1];
        // Left pass from 2nd partition(1) to last partition(binCount - 2)
        for (int i = 1; i < binCount - 1; ++i) {
            lPart[i].bb = bbUnion(lPart[i - 1].bb, bins[i].bb);
            lPart[i].count = lPart[i - 1].count + bins[i].count;
        }
        // Right pass from 2nd last partition(binCount - 3) to 1st partition(0)
        for (int i = binCount - 3; i != -1; --i) {
            rPart[i].bb = bbUnion(rPart[i + 1].bb, bins[i + 1].bb);
            rPart[i].count = rPart[i + 1].count + bins[i + 1].count;
        }

        /* Compute partition costs and find minimum cost */
        tkFloat minCost = TK_INFINITY;
        int minCostPart = 0;
        for (int i = 0; i < binCount - 1; ++i) {
            tkFloat cost =
                lPart[i].count * lPart[i].bb.surfaceArea() + rPart[i].count * rPart[i].bb.surfaceArea();
            if (cost < minCost) {
                minCost = cost;
                minCostPart = i;
            }
        }

        /* Termination criteria met, make leaf node */
        tkFloat noSplitCost = count * bb.surfaceArea();
        if (minCost > noSplitCost || count < maxPCount) {
            nodes[nodeIndex].makeLeaf(bb, count, start);
            return;
        }

        /* Make interior node and recurse */
        int64_t maxNodesLeft = 2 * lPart[minCostPart].count - 1;
        int64_t left = nodeIndex + 1;
        int64_t right = left + maxNodesLeft;
        nodes[nodeIndex].makeInner(bb, left, right, axis);
        PrimitiveUnit* midPtr = std::partition(&pSet[start], &pSet[end - 1] + 1, [=](const PrimitiveUnit& p) {
            int binIndex = k1 * (p.centroid[axis] - k0);
            return binIndex <= minCostPart;
        });
        int64_t mid = midPtr - &pSet[0];
        buildSAH(left, start, mid, pSet);
        buildSAH(right, mid, end, pSet);
    }
}  // namespace TK