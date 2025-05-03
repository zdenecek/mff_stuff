#ifndef IMPLEMENTATION_HPP
#define IMPLEMENTATION_HPP

#include <interface.hpp>
#include <data.hpp>
#include <sycl/sycl.hpp>
#include <vector>
#include <cmath>
#include <cstdint>
#include <cassert>
#include <iostream>

/*
 * Final implementation of the tested program.
 */
template<typename F = float, typename IDX_T = std::uint32_t, typename LEN_T = std::uint32_t>
class ProgramPotential : public IProgramPotential<F, IDX_T, LEN_T> {
public:
    using coord_t  = F;
    using real_t   = coord_t;
    using index_t  = IDX_T;
    using length_t = LEN_T;
    using point_t  = Point<coord_t>;
    using edge_t   = Edge<index_t>;

private:
    sycl::queue                  mQueue;
    size_t                       mPointsCount{0};
    size_t                       mEdgesCount{0};

    sycl::buffer<point_t,1>      mPosBuf;
    sycl::buffer<point_t,1>      mNewPosBuf;
    sycl::buffer<point_t,1>      mVelBuf;
    sycl::buffer<index_t,1>      mNbrIdxBuf;
    sycl::buffer<length_t,1>     mNbrLenBuf;
    sycl::buffer<index_t,1>      mNbrOffBuf;

public:
    ProgramPotential()
        : mQueue(sycl::default_selector_v),
          mPosBuf(1), mNewPosBuf(1), mVelBuf(1),
          mNbrIdxBuf(1), mNbrLenBuf(1), mNbrOffBuf(1) {}

    void initialize(index_t points,
                    const std::vector<edge_t>& edges,
                    const std::vector<length_t>& lengths,
                    index_t /*iterations*/) override {
        mPointsCount = points;
        mEdgesCount  = edges.size();

        mPosBuf    = sycl::buffer<point_t,1>(mPointsCount);
        mNewPosBuf = sycl::buffer<point_t,1>(mPointsCount);
        mVelBuf    = sycl::buffer<point_t,1>(mPointsCount);

        std::vector<size_t> degree(points, 0);
        for (const auto &e : edges) {
            assert(e.p1 < points && e.p2 < points);
            degree[e.p1]++;
            degree[e.p2]++;
        }
        std::vector<index_t> offsets(points + 1);
        offsets[0] = 0;
        for (size_t i = 0; i < points; ++i)
            offsets[i + 1] = offsets[i] + degree[i];
        assert(offsets.back() == mEdgesCount * 2);

        std::vector<index_t> nbrIdx(2 * mEdgesCount);
        std::vector<length_t> nbrLen(2 * mEdgesCount);
        std::vector<index_t> cur = offsets;
        for (size_t i = 0; i < mEdgesCount; ++i) {
            auto u = edges[i].p1;
            auto v = edges[i].p2;
            auto L = lengths[i];
            nbrIdx[cur[u]] = v;
            nbrLen[cur[u]] = L;
            cur[u]++;
            nbrIdx[cur[v]] = u;
            nbrLen[cur[v]] = L;
            cur[v]++;
        }

        // allocation
        mNbrIdxBuf = sycl::buffer<index_t,1>(sycl::range<1>(nbrIdx.size()));
        mNbrLenBuf = sycl::buffer<length_t,1>(sycl::range<1>(nbrLen.size()));
        mNbrOffBuf = sycl::buffer<index_t,1>(sycl::range<1>(offsets.size()));
        {
            sycl::host_accessor<index_t, 1, sycl::access::mode::write> idxAcc(mNbrIdxBuf);
            for (size_t i = 0; i < nbrIdx.size(); ++i) idxAcc[i] = nbrIdx[i];
        }
        {
            sycl::host_accessor<length_t, 1, sycl::access::mode::write> lenAcc(mNbrLenBuf);
            for (size_t i = 0; i < nbrLen.size(); ++i) lenAcc[i] = nbrLen[i];
        }
        {
            sycl::host_accessor<index_t, 1, sycl::access::mode::write> offAcc(mNbrOffBuf);
            for (size_t i = 0; i < offsets.size(); ++i) offAcc[i] = offsets[i];
        }

        {
            sycl::host_accessor<point_t, 1, sycl::access::mode::write> vAcc(mVelBuf);
            for (size_t i = 0; i < mPointsCount; ++i)
                vAcc[i] = point_t{0, 0};
        }
    }

    void iteration(std::vector<point_t>& points) override {
        // tranfer data to device
        {
            sycl::host_accessor<point_t, 1, sycl::access::mode::write> posHost(mPosBuf);
            for (size_t i = 0; i < mPointsCount; ++i)
                posHost[i] = points[i];
        }

        const real_t kRep  = this->mParams.vertexRepulsion;
        const real_t kMass = this->mParams.vertexMass;
        const real_t kComp = this->mParams.edgeCompulsion;
        const real_t kSlow = this->mParams.slowdown;
        const real_t dt    = this->mParams.timeQuantum;
        const real_t eps2  = static_cast<real_t>(1e-4);
        const size_t N     = mPointsCount;

        mQueue.submit([&](sycl::handler &cgh) {
            auto posIn  = mPosBuf.template get_access<sycl::access::mode::read>(cgh);
            auto posOut = mNewPosBuf.template get_access<sycl::access::mode::write>(cgh);
            auto velAcc = mVelBuf.template get_access<sycl::access::mode::read_write>(cgh);
            auto nbrIdx = mNbrIdxBuf.template get_access<sycl::access::mode::read>(cgh);
            auto nbrLen = mNbrLenBuf.template get_access<sycl::access::mode::read>(cgh);
            auto nbrOff = mNbrOffBuf.template get_access<sycl::access::mode::read>(cgh);

            cgh.parallel_for<class CombinedForces>(
                sycl::range<1>(N), [=](sycl::id<1> id) {
                    size_t i = id[0];
                    auto pi = posIn[i];
                    point_t f{0, 0};

                    // model: repulsive forces with saturation
                    for (size_t j = 0; j < N; ++j) {
                        if (j == i) continue;
                        auto pj = posIn[j];
                        real_t dx = pi.x - pj.x;
                        real_t dy = pi.y - pj.y;
                        real_t r2 = dx*dx + dy*dy;
                        r2 = (r2 < eps2) ? eps2 : r2;
                        real_t r  = sycl::sqrt(r2);
                        real_t coef = kRep / (r2 * r);
                        f.x += dx * coef;
                        f.y += dy * coef;
                    }

                    // model: edge compulsion springs
                    index_t start = nbrOff[i];
                    index_t end   = nbrOff[i+1];
                    for (index_t off = start; off < end; ++off) {
                        index_t j = nbrIdx[off];
                        auto pj = posIn[j];
                        real_t dx = pj.x - pi.x;
                        real_t dy = pj.y - pi.y;
                        real_t dist = sycl::sqrt(dx*dx + dy*dy);
                        real_t factor = (dist * kComp) / nbrLen[off];
                        f.x += dx * factor;
                        f.y += dy * factor;
                    }

                    // model: integrate: update velocity and position
                    auto vi = velAcc[i];
                    real_t ax = f.x / kMass;
                    real_t ay = f.y / kMass;
                    real_t nvx = (vi.x + ax*dt) * kSlow;
                    real_t nvy = (vi.y + ay*dt) * kSlow;
                    real_t npx = pi.x + nvx*dt;
                    real_t npy = pi.y + nvy*dt;

                    velAcc[i]  = point_t{nvx, nvy};
                    posOut[i] = point_t{npx, npy};
                }
            );
        });
        mQueue.wait_and_throw();

        // retrieve positions
        {
            sycl::host_accessor<point_t, 1, sycl::access::mode::read> newPos(mNewPosBuf);
            for (size_t i = 0; i < mPointsCount; ++i)
                points[i] = newPos[i];
        }
    }

    void getVelocities(std::vector<point_t>& velocities) override {
        velocities.resize(mPointsCount);
        sycl::host_accessor<point_t, 1, sycl::access::mode::read> vAcc(mVelBuf);
        for (size_t i = 0; i < mPointsCount; ++i)
            velocities[i] = vAcc[i];
    }
};

#endif // IMPLEMENTATION_HPP
