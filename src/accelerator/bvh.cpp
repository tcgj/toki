#include "bvh.hpp"

#include "core/ray.hpp"
#include "region/primitive.hpp"

namespace TK {
    static constexpr tkUInt maxNodeCount = 64;
    static constexpr tkUInt bucketCount = 12;
    struct SAHBucket {
        tkUInt count = 0;
        tkAABBf bb;
    };

    // Construct a BVH based on the provided construction type
    BVH::BVH(std::vector<std::shared_ptr<Primitive>> &primitives,
             ConstructionType type) : primitives(primitives) {
        tkUInt pLen = primitives.size();
        if (pLen == 0)
            return;

        std::vector<PrimitiveData> primData;
        for (tkUInt i = 0; i < pLen; ++i) {
            primData.emplace_back(i, primitives[i]->worldBoundingBox());
        }

        std::vector<std::shared_ptr<Primitive>> primOrdered;
        switch (type) {
            case ConstructionType::SAH:
                root = constructBySAH(primData, 0, pLen, primOrdered);
                break;
            default:
                break;
        }
        primitives.swap(primOrdered);
        primData.clear();
    }

    tkAABBf BVH::worldBoundingBox() const {
        return root != nullptr ? root->boundingBox : tkAABBf();
    }

    // Traverse the BVH until intersection with a primitive
    bool BVH::hasIntersect(const Ray &r) const {
        return intersectNode(r, root, nullptr);
    }

    // Traverse the BVH to find the closest surface interaction
    bool BVH::intersect(const Ray &r, SurfaceInteraction *interaction) const {
        return intersectNode(r, root, interaction);
    }

    // Start traversal from particular node
    bool BVH::intersectNode(const Ray &r, Node *root, SurfaceInteraction *interaction) const {
        // Pre-calculation for aabb intersection test
        tkVec3f invD(1.0 / r.d.x, 1.0 / r.d.y, 1.0 / r.d.z);
        tkInt dirNegative[3] = {invD.x < 0, invD.y < 0, invD.z < 0};

        bool hit = false;
        tkInt stackPtr = 1;
        Node *toVisit[64] = {root};
        Node *curr;
        while (stackPtr > 0) {
            curr = toVisit[--stackPtr];
            if (!curr->boundingBox.hasIntersect(r, invD, dirNegative)) {
                continue;
            }
            // Has intersection, check if leaf/interior
            if (curr->numPrim > 0) {
                if (interaction != nullptr) {
                    for (tkUInt i = 0; i < curr->numPrim; ++i) {
                        if (primitives[curr->primOffset + i]->intersect(r, interaction))
                            hit = true;
                    }
                } else {
                    for (tkUInt i = 0; i < curr->numPrim; ++i) {
                        if (primitives[curr->primOffset + i]->hasIntersect(r))
                            return true;
                    }
                }
            } else {
                if (dirNegative[curr->splitAxis]) {
                    toVisit[stackPtr++] = curr->right;
                    toVisit[stackPtr++] = curr->left;
                } else {
                    toVisit[stackPtr++] = curr->left;
                    toVisit[stackPtr++] = curr->right;
                }
            }
        }
        return hit;
    }

    // Construct a SAH-based BVH
    BVH::Node *BVH::constructBySAH(std::vector<PrimitiveData> &primData, tkUInt start,
        tkUInt end, std::vector<std::shared_ptr<Primitive>> &primOrdered) {
        Node *node = new Node();
        tkUInt count = end - start;
        tkAABBf bb;
        for (tkUInt i = start; i < end; ++i) {
            bb = bbUnion(bb, primData[i].bb);
        }

        // Only 1 primitive, don't split
        if (count == 1) {
            tkUInt offset = primOrdered.size();
            tkUInt pIndex = primData[start].pIndex;
            primOrdered.push_back(primitives[pIndex]);
            node->makeLeaf(count, offset, bb);
            return node;
        }

        tkAABBf centroidBB;
        for (tkUInt i = start; i < end; ++i)
            centroidBB = bbUnion(centroidBB, primData[i].centroid);
        tkInt axis = centroidBB.maxExtent();

        // Centroids are almost at the same point, don't split
        if (centroidBB.maxPt[axis] - centroidBB.minPt[axis] < TK_EPSILON) {
            tkUInt offset = primOrdered.size();
            for (tkUInt i = start; i < end; ++i) {
                tkUInt pIndex = primData[i].pIndex;
                primOrdered.push_back(primitives[pIndex]);
            }
            node->makeLeaf(count, offset, bb);
            return node;
        }

        // initialize buckets for partitioning tests
        SAHBucket buckets[bucketCount];
        for (tkUInt i = start; i < end; ++i) {
            tkUInt index = bucketCount * centroidBB.offset(primData[i].centroid)[axis];
            index = clamp(index, 0, bucketCount - 1);
            buckets[index].count++;
            buckets[index].bb = bbUnion(buckets[index].bb, primData[i].bb);
        }

        // compute partition costs for each n-1 partition
        tkFloat costs[bucketCount - 1];
        for (tkUInt i = 0; i < bucketCount - 1; ++i) {
            tkAABBf b0, b1;
            tkUInt c0 = 0, c1 = 0;
            for (tkUInt j = 0; j <= i; ++j) {
                b0 = bbUnion(b0, buckets[j].bb);
                c0 += buckets[j].count;
            }
            for (tkUInt j = i + 1; j < bucketCount; ++j) {
                b1 = bbUnion(b1, buckets[j].bb);
                c1 += buckets[j].count;
            }
            costs[i] = 0.125 + (c0 * b0.surfaceArea() + c1 * b1.surfaceArea()) / bb.surfaceArea();
        }

        // Get the partition with minimum cost
        tkFloat minCost = costs[0];
        tkInt minCostPartition = 0;
        for (tkUInt i = 0; i < bucketCount - 1; ++i) {
            if (costs[i] < minCost) {
                minCost = costs[i];
                minCostPartition = i;
            }
        }

        // Partition if necessary
        tkFloat noSplitCost = 0.125 + count;
        if (minCost < noSplitCost || count > 64) {
            PrimitiveData *midPtr = std::partition(
                &primData[start], &primData[end - 1] + 1,
                [=](const PrimitiveData &data) {
                    tkUInt index = bucketCount * centroidBB.offset(data.centroid)[axis];
                    return clamp(index, 0, bucketCount) <= minCostPartition;
                });
            tkUInt mid = midPtr - &primData[0];

            // make interior node
            Node *left, *right;
            left = constructBySAH(primData, start, mid, primOrdered);
            right = constructBySAH(primData, mid, end, primOrdered);
            node->makeInterior(axis, left, right);
        } else {
            // make leaf node
            tkUInt offset = primOrdered.size();
            tkUInt pIndex = primData[start].pIndex;
            primOrdered.push_back(primitives[pIndex]);
            node->makeLeaf(count, offset, bb);
        }
        return node;
    }
} // namespace TK