
#include "precomp.hpp"
#include "undistort.hpp"
#include "opencv2/core/hal/intrin.hpp"

namespace cv
{

int initUndistortRectifyMapLine_VSX(float* m1f, float* m2f, short* m1, ushort* m2, double* matTilt, const double* ir,
                                    double& _x, double& _y, double& _w, int width, int m1type,
                                    double k1, double k2, double k3, double k4, double k5, double k6,
                                    double p1, double p2, double s1, double s2, double s3, double s4,
                                    double u0, double v0, double fx, double fy)
{
    int j = 0;
    static const v_float64x2 zero = v_setall_f64(0.0);
    static const v_float64x2 __one = v_setall_f64(1.0);
    static const v_float64x2 __two = v_setall_f64(2.0);

    const v_float64x2 __matTilt_00 = v_setall_f64(matTilt[0]);
    const v_float64x2 __matTilt_10 = v_setall_f64(matTilt[3]);
    const v_float64x2 __matTilt_20 = v_setall_f64(matTilt[6]);

    const v_float64x2 __matTilt_01 = v_setall_f64(matTilt[1]);
    const v_float64x2 __matTilt_11 = v_setall_f64(matTilt[4]);
    const v_float64x2 __matTilt_21 = v_setall_f64(matTilt[7]);

    const v_float64x2 __matTilt_02 = v_setall_f64(matTilt[2]);
    const v_float64x2 __matTilt_12 = v_setall_f64(matTilt[5]);
    const v_float64x2 __matTilt_22 = v_setall_f64(matTilt[8]);

    for (; j <= width - 4; j += 4, _x += 4 * ir[0], _y += 4 * ir[3], _w += 4 * ir[6])
    {
        // Question: Should we load the constants first?
        v_float64x2 __w10 = v_float64x2(1/_w, 1/(_w + ir[6]));
        v_float64x2 __w11 = v_float64x2(1/(_w + 2 * ir[6]), 1/(_w + 3 * ir[6]) );
        v_float64x2 __x10 = __w10 * (v_float64x2(_x, _x + ir[0]));
        v_float64x2 __x11 = __w11 * (v_float64x2(_x + 2 * ir[0], _x + 3 * ir[0]));
        v_float64x2 __y10 = __w10 * (v_float64x2(_y, _y + ir[3]));
        v_float64x2 __y11 = __w11 * (v_float64x2(_y + 2 * ir[3], _y + 3 * ir[3]));

        v_float64x2 __x20 = __x10 * __x10;
        v_float64x2 __x21 = __x11 * __x11;
        v_float64x2 __y20 = __y10 * __y10;
        v_float64x2 __y21 = __y11 * __y11;
        v_float64x2 __r20 = __x20 + __y20;
        v_float64x2 __r21 = __x21 + __y21;
        v_float64x2 __2xy0 = __two * __x10 * __y10;
        v_float64x2 __2xy1 = __two * __x11 * __y11;

        v_float64x2 kr0 = (((v_setall_f64(k3) * __r20 + v_setall_f64(k2)) * __r20 + v_setall_f64(k1)) * __r20 + __one)
                          / (((v_setall_f64(k6) * __r20 + v_setall_f64(k5)) * __r20 + v_setall_f64(k4)) * __r20 + __one);
        v_float64x2 kr1 = (((v_setall_f64(k3) * __r21 + v_setall_f64(k2)) * __r21 + v_setall_f64(k1)) * __r21 + __one)
                          / (((v_setall_f64(k6) * __r21 + v_setall_f64(k5)) * __r21 + v_setall_f64(k4)) * __r21 + __one);

        v_float64x2 r22a = __r20 * __r20;
        v_float64x2 r22b = __r21 * __r21;

        v_float64x2 xd0 = __x10 * kr0 +
                         (v_setall_f64(p1) * __2xy0 + v_setall_f64(p2) * (__two * __x20 + __r20)
                        +(v_setall_f64(s1) * __r20 + v_setall_f64(s2) * r22a) );
        v_float64x2 xd1 = __x11 * kr1 +
                         (v_setall_f64(p1) * __2xy1 + v_setall_f64(p2) *(__two * __x21 + __r21)
                        +(v_setall_f64(s1) * __r21 + v_setall_f64(s2) * r22b) );

        v_float64x2 yd0 = __y10 * kr0 +
                         ((v_setall_f64(p1) *  (__two * __y20 + __r20) + v_setall_f64(p2) * __2xy0 )
                        +(v_setall_f64(s3) * __r20 + (v_setall_f64(s4) * r22a)) );
        v_float64x2 yd1 = __y11 * kr1 +
                         ((v_setall_f64(p1) *  (__two * __y21 + __r21) + v_setall_f64(p2) * __2xy1 )
                        +(v_setall_f64(s3) * __r21 + (v_setall_f64(s4) * r22b)) );

        v_float64x2 __vecTilt20 = __matTilt_20 * xd0 + ( __matTilt_21 * yd0 + __matTilt_22);
        v_float64x2 __vecTilt21 = __matTilt_20 * xd1 + ( __matTilt_21 * yd1 + __matTilt_22);

        v_float64x2 mask0 = __vecTilt20 == zero;
        v_float64x2 mask1 = __vecTilt21 == zero;

        v_float64x2 __invProj0 = v_select(mask0, __one, __one/__vecTilt20 );
        v_float64x2 __invProj1 = v_select(mask1, __one, __one/__vecTilt21 );

        v_float64x2 __u0 = __matTilt_00 * xd0 + (__matTilt_01 * yd0 + __matTilt_02);
        v_float64x2 __u1 = __matTilt_00 * xd1 + (__matTilt_01 * yd1 + __matTilt_02);
        v_float64x2 __v0 = __matTilt_10 * xd0 + (__matTilt_11 * yd0 + __matTilt_12);
        v_float64x2 __v1 = __matTilt_10 * xd1 + (__matTilt_11 * yd1 + __matTilt_12);

        __u0 = v_setall_f64(fx) * __invProj0 * __u0 + v_setall_f64(u0);
        __u1 = v_setall_f64(fx) * __invProj1 * __u1 + v_setall_f64(u0);
        __v0 = v_setall_f64(fy) * __invProj0 * __v0 + v_setall_f64(v0);
        __v1 = v_setall_f64(fy) * __invProj1 * __v1 + v_setall_f64(v0);

        if (m1type == CV_32FC1)
        {
           v_store(&m1f[j], v_combine_low(v_cvt_f32(__u0), v_cvt_f32(__u1)));
           v_store(&m2f[j], v_combine_low(v_cvt_f32(__v0), v_cvt_f32(__v1)));
        }
        else if(m1type == CV_32FC2)
        {
           v_float32x4 __u_f = v_combine_low(v_cvt_f32(__u0), v_cvt_f32(__u1));
           v_float32x4 __v_f = v_combine_low(v_cvt_f32(__v0), v_cvt_f32(__v1));

           v_store_interleave(&m1f[j*2], __u_f, __v_f);
        }
        else //if(m1type == CV_16SC2)
        {
           v_int32x4 v_iu = v_combine_low(v_round(__u0 * v_setall_f64(INTER_TAB_SIZE)) , 
                                          v_round(__u1 * v_setall_f64(INTER_TAB_SIZE)));
           v_int32x4 v_iv = v_combine_low(v_round(__v0 * v_setall_f64(INTER_TAB_SIZE)) , 
                                          v_round(__v1 * v_setall_f64(INTER_TAB_SIZE)));

           v_int16x8 iu_s = v_pack(v_iu >> INTER_BITS, v_setall_s32(0));
           v_int16x8 iv_s = v_pack(v_iv >> INTER_BITS, v_setall_s32(0));
           v_zip(iu_s, iv_s, iu_s, iv_s);
           v_store(&m1[j*2], iu_s);
           v_store_low(&m2[j]  , v_pack_u( (v_iv & v_setall_s32(INTER_TAB_SIZE-1))* v_setall_s32(INTER_TAB_SIZE) 
                                         + (v_iu & v_setall_s32(INTER_TAB_SIZE-1)) , v_setall_s32(0)));
        }
      }

        return j;
    }

}

    /*  End of file  */
