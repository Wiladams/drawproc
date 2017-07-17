// There are various sources for high precision numbers.  
// The well known CRC books are one.  
//
// The ones used here come variously from the Graphics Gems
// books, as well as the glm library, or direct derivation
//
// the 'G_' prefix is used to avoid potential conflict with the 'M_' prefix that might
// be used in math.h

#define G_PI     3.141592653589793238462643383279502884197169399375105820975
#define G_E      2.718281828459045235360287471352662497757247093699959574967
#define G_SQRT2  1.414213562373095048801688724209698078569671875376948073177
#define G_SQRT3  1.732050807568877293527446341505872366942805253810380628056
#define G_GOLDEN 1.618033988749894848204586834365638117720309179805762862135

// derived constants
#define G_TWO_PI	(2.0*G_PI)
#define G_PITIMES2  (2.0*G_PI)
#define G_PIOVER2   (0.5*G_PI)
#define G_PIOVER4	(0.25*G_PI)
#define G_ONE_OVER_PI	(1.0/G_PI)
//#define G_ONE_OVER_PI	0.318309886183790671537767526745028724
#define G_ROOT_PI		1.772453850905516027

#define G_DTOR      (G_PI/180.0)
#define G_RTOD      (180.0/G_PI)

// macro and constant for base 2 logarithm
#define G_LN2    0.693147180559945309417232121458176568075500134360255254121
#define G_LOG2(val) (log(val)*(1.0/G_LN_2))

#define G_EULER			0.577215664901532860606

// A decently small value
static const double  EPSILON = 0.0000000001;
