#ifndef KMEANS_IMPLEMENTATION_HPP
#define KMEANS_IMPLEMENTATION_HPP

#include <vector>
#include <cstdint>
#include <tbb/parallel_for.h>
#include <tbb/combinable.h>

template<typename POINT = point_t, typename ASGN = std::uint8_t, bool DEBUG = false>
class KMeans : public IKMeans<POINT, ASGN, DEBUG>
{
private:
    typedef typename POINT::coord_t coord_t;
    std::vector<POINT> sums;
    std::vector<std::size_t> counts;

    static coord_t distance(const POINT &point, const POINT &centroid)
    {
        std::int64_t dx = static_cast<std::int64_t>(point.x) - static_cast<std::int64_t>(centroid.x);
        std::int64_t dy = static_cast<std::int64_t>(point.y) - static_cast<std::int64_t>(centroid.y);
        return static_cast<coord_t>(dx * dx + dy * dy);
    }

    static std::size_t getNearestCluster(const POINT &point, const std::vector<POINT> &centroids)
    {
        coord_t minDist = distance(point, centroids[0]);
        std::size_t nearest = 0;
        for (std::size_t i = 1; i < centroids.size(); ++i) {
            coord_t d = distance(point, centroids[i]);
            if (d < minDist) {
                minDist = d;
                nearest = i;
            }
        }
        return nearest;
    }

public:
    virtual void init(std::size_t points, std::size_t k, std::size_t iters)
    {
        sums.resize(k);
        counts.resize(k);
    }

    virtual void compute(const std::vector<POINT> &points, std::size_t k, std::size_t iters,
                         std::vector<POINT> &centroids, std::vector<ASGN> &assignments)
    {
        centroids.resize(k);
        assignments.resize(points.size());
        for (std::size_t i = 0; i < k; ++i)
            centroids[i] = points[i];

        while (iters--) {
            for (std::size_t i = 0; i < k; ++i) {
                sums[i].x = 0;
                sums[i].y = 0;
                counts[i] = 0;
            }

            tbb::combinable<std::pair<std::vector<POINT>, std::vector<std::size_t>>> local_acc(
                [k]() -> std::pair<std::vector<POINT>, std::vector<std::size_t>> {
                    std::vector<POINT> localSums(k);
                    for (std::size_t j = 0; j < k; ++j) {
                        localSums[j].x = 0;
                        localSums[j].y = 0;
                    }
                    std::vector<std::size_t> localCounts(k, 0);
                    return std::make_pair(localSums, localCounts);
                }
            );

            tbb::parallel_for(tbb::blocked_range<std::size_t>(0, points.size()),
                [&](const tbb::blocked_range<std::size_t>& r) {
                    auto &localPair = local_acc.local();
                    auto &localSums = localPair.first;
                    auto &localCounts = localPair.second;
                    for (std::size_t i = r.begin(); i < r.end(); ++i) {
                        std::size_t nearest = getNearestCluster(points[i], centroids);
                        assignments[i] = static_cast<ASGN>(nearest);
                        localSums[nearest].x += points[i].x;
                        localSums[nearest].y += points[i].y;
                        localCounts[nearest] += 1;
                    }
                }
            );

            local_acc.combine_each([&](const std::pair<std::vector<POINT>, std::vector<std::size_t>> &localPair) {
                for (std::size_t j = 0; j < k; j++) {
                    sums[j].x += localPair.first[j].x;
                    sums[j].y += localPair.first[j].y;
                    counts[j] += localPair.second[j];
                }
            });

            for (std::size_t i = 0; i < k; ++i) {
                if (counts[i] != 0) {
                    centroids[i].x = sums[i].x / static_cast<std::int64_t>(counts[i]);
                    centroids[i].y = sums[i].y / static_cast<std::int64_t>(counts[i]);
                }
            }
        }
    }
};

#endif
