
constexpr double piD = 3.14159265358979323846;
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = float(2.0 * piD);

constexpr float invPi = float(1.0 / piD);
constexpr float invTwoPi = float(0.5 / piD);

constexpr float degToRad = float(piD / 180.0);
constexpr float radToDeg = float(180.0 / piD);

typedef float(*SimpleTrigCall)(float x);
typedef float(*TwoParamFoldCall)(float x,float* out_sign);

// Fold input to range [-pi/2,pi/2]
float fold_sin_input(float x);

// Fold input to range [-pi/2,pi/2]
float fold_sin_input_v2(float x, float* out_sign);

float fold_sin_input_v3(float x);

// Valid for any range
float sin_poly3(float x);

// Valid for any range
float sin_poly5(float x);

// Valid for any range
float sin_poly3_v2(float x);

// Valid for any range
float sin_poly5_v2(float x);

// Only valid for range [-pi/2,pi/2]
float sin_poly3_principal(float x);

// Only valid for range [-pi/2,pi/2]
float sin_poly5_principal(float x);
