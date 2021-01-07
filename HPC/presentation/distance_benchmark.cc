#include "celero/Celero.h"
#include <random>
#include <x86intrin.h>

constexpr size_t nbr_samples = 100;
constexpr size_t nbr_iterations = 0;
constexpr size_t list_length = 16; /* Power of 2 */

auto rd = std::default_random_engine();

auto udist = std::uniform_int_distribution<int32_t>(
    std::numeric_limits<int32_t>::min(),
    std::numeric_limits<int32_t>::max());

class fill_vector
    : public celero::TestFixture {
public:
    int32_t r1[3];
    int32_t r2[3];
    uint32_t distance;

public:
    fill_vector()
    {
	for (size_t i = 0; i < 3; ++i) {
	    r1[i] = udist(rd);
	    r2[i] = udist(rd);
	}
    }

    ~fill_vector()
    {
    }
};

class fill_list
    : public celero::TestFixture {
public:
    int32_t r1[list_length][3];
    int32_t r2[list_length][3];
    uint32_t distance[list_length];

public:
    fill_list()
    {
	for (size_t i = 0; i < 3; ++i) {
	    for (size_t j = 0; j < list_length; ++j) {
		r1[j][i] = udist(rd);
		r2[j][i] = udist(rd);
	    }
	}
    }

    ~fill_list()
    {
    }
};

CELERO_MAIN

/* BASELINE_F(along_3vec, individual, fill_vector, nbr_samples, nbr_iterations) */
/* { */
/*     int32_t r12[3]; */
/*     r12[0] = r1[0] - r2[0]; */
/*     r12[1] = r1[1] - r2[1]; */
/*     r12[2] = r1[2] - r2[2]; */

/*     distance = (uint32_t)sqrtf(r12[0] * r12[0] + r12[1] * r12[1] + r12[2] * r12[2]); */
/*     celero::DoNotOptimizeAway(distance); */
/* } */

BASELINE_F(along_3vec, forloop, fill_vector, nbr_samples, nbr_iterations)
{
    int32_t r12[3];
    for (int i = 0; i < 3; ++i) {
	r12[i] = r1[i] - r2[i];
    }

    distance = (uint32_t)sqrtf(r12[0] * r12[0] + r12[1] * r12[1] + r12[2] * r12[2]);
    celero::DoNotOptimizeAway(distance);
}

/* BENCHMARK_F(along_3vec, sub2, fill_vector, nbr_samples, nbr_iterations) */
/* { */
/*     __m128i r1_ = _mm_loadu_si128((__m128i*)(r1)); */
/*     __m128i r2_ = _mm_loadu_si128((__m128i*)(r2)); */
/*     uint32_t r12[3]; */

/*     _mm_storeu_si128((__m128i*)(r12), _mm_sub_epi32(r1_, r2_)); */
/*     _mm_storeu_si128((__m128i*)(r12 + 2), _mm_sub_epi32(r1_ + 2, r2_ + 2)); */

/*     distance = (uint32_t)sqrtf(r12[0] * r12[0] + r12[1] * r12[1] + r12[2] * r12[2]); */
/*     celero::DoNotOptimizeAway(distance); */
/* } */

/* BENCHMARK_F(along_3vec, sub4, fill_vector, nbr_samples, nbr_iterations) */
/* { */
/*     //TODO: wtf is avx? do I need to have -mavx flag also? */
/*     __m256i r1_ = _mm256_loadu_si256((__m256i const*)(r1)); */
/*     __m256i r2_ = _mm256_loadu_si256((__m256i const*)(r2)); */
/*     uint32_t r12[3]; */

/*     _mm256_storeu_si256((__m256i*)(r12), _mm256_sub_epi32(r1_, r2_)); */

/*     distance = (uint32_t)sqrtf(r12[0] * r12[0] + r12[1] * r12[1] + r12[2] * r12[2]); */
/*     celero::DoNotOptimizeAway(distance); */
/* } */

/* BENCHMARK_F(along_3vec, sub2_mul2, fill_vector, nbr_samples, nbr_iterations) */
/* { */
/*     __m128i r1_ = _mm_loadu_si128((__m128i*)(r1)); */
/*     __m128i r2_ = _mm_loadu_si128((__m128i*)(r2)); */
/*     __m128i r12_; */
/*     uint32_t rr12[3]; */

/*     r12_ = _mm_sub_epi32(r1_, r2_); */

/*     _mm_storeu_si128((__m128i*)(rr12), _mm_mullo_epi32(r12_, r12_)); */

/*     distance = (uint32_t)sqrtf(rr12[0] + rr12[1] + (r1[2] - r2[2]) * (r1[2] - r2[2])); */
/*     celero::DoNotOptimizeAway(distance); */
/* } */

BENCHMARK_F(along_3vec, sub4_mul4, fill_vector, nbr_samples, nbr_iterations)
{
    //TODO: wtf is avx? do I need to have -mavx flag also?
    __m128i r1_ = _mm_loadu_si128((__m128i*)(r1));
    __m128i r2_ = _mm_loadu_si128((__m128i*)(r2));
    __m128i r12_;
    uint32_t rr12[3];

    r12_ = _mm_sub_epi32(r1_, r2_);

    _mm_storeu_si128((__m128i*)(rr12), _mm_mul_epi32(r12_, r12_));

    distance = (uint32_t)sqrtf(rr12[0] + rr12[1] + rr12[2]);
    celero::DoNotOptimizeAway(distance);
}

/* BENCHMARK_F(along_3vec, sub2_mul2_hadd, fill_vector, nbr_samples, nbr_iterations) */
/* { */
/*     __m128i r1_ = _mm_loadu_si128((__m128i*)(r1)); */
/*     __m128i r2_ = _mm_loadu_si128((__m128i*)(r2)); */
/*     __m128i r12_; */
/*     __m128i rr12_; */
/*     uint32_t rr12[3]; */

/*     r12_ = _mm_sub_epi32(r1_, r2_); */

/*     rr12_ = _mm_mullo_epi32(r12_, r12_); */
/*     _mm_hadd_epi32(rr12_, rr12_); */

/*     _mm_storeu_si128((__m128i*)(rr12), _mm_mullo_epi32(r12_, r12_)); */

/*     distance = (uint32_t)sqrtf(rr12[0] + rr12[1] + (r1[2] - r2[2]) * (r1[2] - r2[2])); */
/*     celero::DoNotOptimizeAway(distance); */
/* } */

/* BENCHMARK_F(along_3vec, fmadd, fill_vector, nbr_samples, nbr_iterations) */
/* { */
/*     //TODO: wtf is avx? do I need to have -mavx flag also? */
/*     __m256i r1_ = _mm256_loadu_si256((__m256i const*)(r1)); */
/*     __m256i r2_ = _mm256_loadu_si256((__m256i const*)(r2)); */
/*     __m256i r12_; */
/*     uint32_t rr12[3]; */

/*     r12_ = _mm256_sub_epi32(r1_, r2_); */

/*     _mm256_storeu_si256((__m256i*)(rr12), _mm256_mullo_epi32(r12_, r12_)); */

/*     distance = (uint32_t)sqrtf(rr12[0] + rr12[1] + rr12[2]); */
/*     celero::DoNotOptimizeAway(distance); */
/* } */

/* ###################################################################################################################################################################### */

BASELINE_F(along_list, forloop, fill_list, nbr_samples, nbr_iterations)
{
    int32_t r12[list_length][3];
    for (size_t i = 0; i < list_length; i++) {
	r12[i][0] = r1[i][0] - r2[i][0];
	r12[i][1] = r1[i][1] - r2[i][1];
	r12[i][2] = r1[i][2] - r2[i][2];

	distance[i] = (uint32_t)sqrtf(r12[i][0] * r12[i][0] + r12[i][1] * r12[i][1] + r12[i][2] * r12[i][2]);
    }
    celero::DoNotOptimizeAway(distance);
}

BENCHMARK_F(along_list, sub4_mul4_add4_sqrtf4, fill_list, nbr_samples, nbr_iterations)
{
    for (size_t i = 0; i < list_length; i += 4) {
	int32_t x1_adresses[] = { r1[i][0], (r1 + 1)[i][0], (r1 + 2)[i][0], (r1 + 3)[i][0] };
	__m128i x1 = _mm_loadu_si128((__m128i*)(x1_adresses));
	int32_t y1_adresses[] = { r1[i][1], (r1 + 1)[i][1], (r1 + 2)[i][1], (r1 + 3)[i][1] };
	__m128i y1 = _mm_loadu_si128((__m128i*)(y1_adresses));
	int32_t z1_adresses[] = { r1[i][2], (r1 + 1)[i][2], (r1 + 2)[i][2], (r1 + 3)[i][2] };
	__m128i z1 = _mm_loadu_si128((__m128i*)(z1_adresses));

	int32_t x2_adresses[] = { r2[i][0], (r2 + 1)[i][0], (r2 + 2)[i][0], (r2 + 3)[i][0] };
	__m128i x2 = _mm_loadu_si128((__m128i*)(x2_adresses));
	int32_t y2_adresses[] = { r2[i][1], (r2 + 1)[i][1], (r2 + 2)[i][1], (r2 + 3)[i][1] };
	__m128i y2 = _mm_loadu_si128((__m128i*)(y2_adresses));
	int32_t z2_adresses[] = { r2[i][2], (r2 + 1)[i][2], (r2 + 2)[i][2], (r2 + 3)[i][2] };
	__m128i z2 = _mm_loadu_si128((__m128i*)(z2_adresses));

	__m128i x_diff = _mm_sub_epi32(x1, x2);
	__m128i y_diff = _mm_sub_epi32(y1, y2);
	__m128i z_diff = _mm_sub_epi32(z1, z2);

	__m128i x_diff_squared = _mm_mul_epi32(x_diff, x_diff);
	__m128i y_diff_squared = _mm_mul_epi32(y_diff, y_diff);
	__m128i z_diff_squared = _mm_mul_epi32(z_diff, z_diff);
	__m128i distance_squared_int;
	__m128 distance_squared_float;
	__m128 distance_float;

	distance_squared_int = _mm_add_epi32(x_diff_squared, _mm_add_epi32(y_diff_squared, z_diff_squared)); /* Add differences squared */
	distance_squared_float = _mm_cvtepi32_ps(distance_squared_int); /* Convert to float */
	distance_float = _mm_sqrt_ps(distance_squared_float); /* Take sqrtf() */
	_mm_storeu_si128((__m128i*)(distance + i), _mm_cvtps_epi32(distance_float)); /* Convert back to int32 */
    }
    celero::DoNotOptimizeAway(distance);
}

BENCHMARK_F(along_list, sub8_mul8_add8_sqrtf8, fill_list, nbr_samples, nbr_iterations)
{
    for (size_t i = 0; i < list_length; i += 8) {
	int32_t x1_adresses[] = {r1[i][0], (r1 + 1)[i][0], (r1 + 2)[i][0], (r1 + 3)[i][0], (r1 + 4)[i][0], (r1 + 5)[i][0], (r1 + 6)[i][0], (r1 + 7)[i][0]};
	__m256i x1 = _mm256_loadu_si256((__m256i*)(x1_adresses));
	int32_t y1_adresses[] = {r1[i][1], (r1 + 1)[i][1], (r1 + 2)[i][1], (r1 + 3)[i][1], (r1 + 4)[i][1], (r1 + 5)[i][1], (r1 + 6)[i][1], (r1 + 7)[i][1]};
	__m256i y1 = _mm256_loadu_si256((__m256i*)(y1_adresses));
	int32_t z1_adresses[] = {r1[i][2], (r1 + 1)[i][2], (r1 + 2)[i][2], (r1 + 3)[i][2], (r1 + 4)[i][2], (r1 + 5)[i][2], (r1 + 6)[i][2], (r1 + 7)[i][2]};
	__m256i z1 = _mm256_loadu_si256((__m256i*)(z1_adresses));

	int32_t x2_adresses[] = {r2[i][0], (r2 + 1)[i][0], (r2 + 2)[i][0], (r2 + 3)[i][0], (r2 + 4)[i][0], (r2 + 5)[i][0], (r2 + 6)[i][0], (r2 + 7)[i][0]};
	__m256i x2 = _mm256_loadu_si256((__m256i*)(x2_adresses));
	int32_t y2_adresses[] = {r2[i][1], (r2 + 1)[i][1], (r2 + 2)[i][1], (r2 + 3)[i][1], (r2 + 4)[i][1], (r2 + 5)[i][1], (r2 + 6)[i][1], (r2 + 7)[i][1]};
	__m256i y2 = _mm256_loadu_si256((__m256i*)(y2_adresses));
	int32_t z2_adresses[] = {r2[i][2], (r2 + 1)[i][2], (r2 + 2)[i][2], (r2 + 3)[i][2], (r2 + 4)[i][2], (r2 + 5)[i][2], (r2 + 6)[i][2], (r2 + 7)[i][2]};
	__m256i z2 = _mm256_loadu_si256((__m256i*)(z2_adresses));

	__m256i x_diff = _mm256_sub_epi32(x1, x2);
	__m256i y_diff = _mm256_sub_epi32(y1, y2);
	__m256i z_diff = _mm256_sub_epi32(z1, z2);

	__m256i x_diff_squared = _mm256_mul_epi32(x_diff, x_diff);
	__m256i y_diff_squared = _mm256_mul_epi32(y_diff, y_diff);
	__m256i z_diff_squared = _mm256_mul_epi32(z_diff, z_diff);
	__m256i distance_squared_int;
	__m256 distance_squared_float;
	__m256 distance_float;

	distance_squared_int = _mm256_add_epi32(x_diff_squared, _mm256_add_epi32(y_diff_squared, z_diff_squared)); /* Add differences squared */
	distance_squared_float = _mm256_cvtepi32_ps(distance_squared_int); /* Convert to float */
	distance_float = _mm256_sqrt_ps(distance_squared_float); /* Take sqrtf() */
	_mm256_storeu_si256((__m256i*)(distance + i), _mm256_cvtps_epi32(distance_float)); /* Convert back to int32 */
    }
    celero::DoNotOptimizeAway(distance);
}

BENCHMARK_F(along_list, sub16_mul16_add16_sqrtf8, fill_list, nbr_samples, nbr_iterations)
{
    for (size_t i = 0; i < list_length; i += 16) {
	int32_t x1_adresses[] = {r1[i][0], (r1 + 1)[i][0], (r1 + 2)[i][0], (r1 + 3)[i][0], (r1 + 4)[i][0], (r1 + 5)[i][0], (r1 + 6)[i][0], (r1 + 7)[i][0], (r1 + 8)[i][0], (r1 + 9)[i][0], (r1 + 10)[i][0], (r1 + 11)[i][0], (r1 + 12)[i][0], (r1 + 13)[i][0], (r1 + 14)[i][0], (r1 + 15)[i][0]};
	__m512i x1 = _mm512_loadu_si512((__m512i*)(x1_adresses));
	int32_t y1_adresses[] = {r1[i][1], (r1 + 1)[i][1], (r1 + 2)[i][1], (r1 + 3)[i][1], (r1 + 4)[i][1], (r1 + 5)[i][1], (r1 + 6)[i][1], (r1 + 7)[i][1], (r1 + 8)[i][1], (r1 + 9)[i][1], (r1 + 10)[i][1], (r1 + 11)[i][1], (r1 + 12)[i][1], (r1 + 13)[i][1], (r1 + 14)[i][1], (r1 + 15)[i][1]};
	__m512i y1 = _mm512_loadu_si512((__m512i*)(y1_adresses));
	int32_t z1_adresses[] = {r1[i][2], (r1 + 1)[i][2], (r1 + 2)[i][2], (r1 + 3)[i][2], (r1 + 4)[i][2], (r1 + 5)[i][2], (r1 + 6)[i][2], (r1 + 7)[i][2], (r1 + 8)[i][2], (r1 + 9)[i][2], (r1 + 10)[i][2], (r1 + 11)[i][2], (r1 + 12)[i][2], (r1 + 13)[i][2], (r1 + 14)[i][2], (r1 + 15)[i][2]};
	__m512i z1 = _mm512_loadu_si512((__m512i*)(z1_adresses));

	int32_t x2_adresses[] = {r2[i][0], (r2 + 1)[i][0], (r2 + 2)[i][0], (r2 + 3)[i][0], (r2 + 4)[i][0], (r2 + 5)[i][0], (r2 + 6)[i][0], (r2 + 7)[i][0], (r2 + 8)[i][0], (r2 + 9)[i][0], (r2 + 10)[i][0], (r2 + 11)[i][0], (r2 + 12)[i][0], (r2 + 13)[i][0], (r2 + 14)[i][0], (r2 + 15)[i][0]};
	__m512i x2 = _mm512_loadu_si512((__m512i*)(x2_adresses));
	int32_t y2_adresses[] = {r2[i][1], (r2 + 1)[i][1], (r2 + 2)[i][1], (r2 + 3)[i][1], (r2 + 4)[i][1], (r2 + 5)[i][1], (r2 + 6)[i][1], (r2 + 7)[i][1], (r2 + 8)[i][1], (r2 + 9)[i][1], (r2 + 10)[i][1], (r2 + 11)[i][1], (r2 + 12)[i][1], (r2 + 13)[i][1], (r2 + 14)[i][1], (r2 + 15)[i][1]};
	__m512i y2 = _mm512_loadu_si512((__m512i*)(y2_adresses));
	int32_t z2_adresses[] = {r2[i][2], (r2 + 1)[i][2], (r2 + 2)[i][2], (r2 + 3)[i][2], (r2 + 4)[i][2], (r2 + 5)[i][2], (r2 + 6)[i][2], (r2 + 7)[i][2], (r2 + 8)[i][2], (r2 + 9)[i][2], (r2 + 10)[i][2], (r2 + 11)[i][2], (r2 + 12)[i][2], (r2 + 13)[i][2], (r2 + 14)[i][2], (r2 + 15)[i][2]};
	__m512i z2 = _mm512_loadu_si512((__m512i*)(z2_adresses));

	__m512i x_diff = _mm512_sub_epi32(x1, x2);
	__m512i y_diff = _mm512_sub_epi32(y1, y2);
	__m512i z_diff = _mm512_sub_epi32(z1, z2);

	__m512i x_diff_squared = _mm512_mul_epi32(x_diff, x_diff);
	__m512i y_diff_squared = _mm512_mul_epi32(y_diff, y_diff);
	__m512i z_diff_squared = _mm512_mul_epi32(z_diff, z_diff);

	__m512i distance_squared_int;
	__m512 distance_squared_float;
	__m512 distance_float;

	distance_squared_int = _mm512_add_epi32(x_diff_squared, _mm512_add_epi32(y_diff_squared, z_diff_squared)); /* Add differences squared */
	distance_squared_float = _mm512_cvtepi32_ps(distance_squared_int); /* Convert to float */
	distance_float = _mm512_sqrt_ps(distance_squared_float); /* Take sqrtf() */
	_mm512_storeu_si512((__m256i*)(distance + i), _mm512_cvtps_epi32(distance_float)); /* Convert back to int32 */
 
    }
    celero::DoNotOptimizeAway(distance);
}
