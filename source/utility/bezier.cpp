/*!
 * @file
 * 
 * @brief ベジェクラスの実態
 *
 * @author sawasaka
 *
 * @since 2013/03/27
 *
 * @copyright
 */
#include "Bezier.h"

namespace Bezier
{
	float Calc(float x, float x1, float y1, float x2, float y2)
	{
		static const float eps = 0.001f;

		float down = 0.0f;
		float up = 1.0f;

		float t;
		while (true) {
			t = (down + up) / 2;
			const float p = 3 * (1 - t) * (1 - t) * t * x1 + 3 * (1 - t) * t * t * x2 + t * t * t;
			if (p < x-eps) {
				down = t;
			} else if (p > x+eps) {
				up = t;
			} else {
				break;
			}
		}

		return 3 * (1 - t) * (1 - t) * t * y1 + 3 * (1 - t) * t * t * y2 + t * t * t;
	}
}
