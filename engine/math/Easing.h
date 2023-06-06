#pragma once

/// 元のコード

namespace Easing {
	namespace Back {
		float easeIn (float t, float b, float c, float d);
		float easeOut(float t, float b, float c, float d);
		float easeInOut(float t, float b, float c, float d);
	}

	namespace Bounce {
		float easeIn(float t, float b, float c, float d);
		float easeOut(float t, float b, float c, float d);
		float easeInOut(float t, float b, float c, float d);
	}

	namespace Circ {
		float easeIn(float t, float b, float c, float d);
		float easeOut(float t, float b, float c, float d);
		float easeInOut(float t, float b, float c, float d);
	}

	namespace Quint {
		float easeIn(float t, float b, float c, float d);
		float easeOut(float t, float b, float c, float d);
		float easeInOut(float t, float b, float c, float d);
	}

	namespace Cubic {
		float easeIn(float t, float b, float c, float d);
		float easeOut(float t, float b, float c, float d);
		float easeInOut(float t, float b, float c, float d);
	}
}

/// 改変後コード

namespace Easing {
	namespace Back {
		float easeIn(float start, float end, float t);
		float easeOut(float start, float end, float t);
		float easeInOut(float start, float end, float t);
	}

	namespace Bounce {
		float easeIn(float t, float b, float c, float d);
		float easeOut(float t, float b, float c, float d);
		float easeInOut(float t, float b, float c, float d);
	}

	namespace Circ {
		float easeIn(float start, float end, float t);
		float easeOut(float start, float end, float t);
		float easeInOut(float start, float end, float t);
	}

	namespace Quint {
		float easeIn(float start, float end, float t);
		float easeOut(float start, float end, float t);
		float easeInOut(float start, float end, float t);
	}

	namespace Cubic {
		float easeIn(float start, float end, float t);
		float easeOut(float start, float end, float t);
		float easeInOut(float start, float end, float t);
	}
}