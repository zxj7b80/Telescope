/* surround declarations with P_() to include prototypes if it looks likes
 * compiler should handle them.
 */
#ifndef P_
#if defined(__STDC__) || defined(__cplusplus) || NeedFunctionPrototypes
#define P_(s) s
#else
#define P_(s) ()
#endif
#endif /* P_ */

#ifndef PI
#define	PI		3.141592653589793
#endif

/* conversions among hours (of ra), degrees and radians. */
#define	degrad(x)	((x)*PI/180.)
#define	raddeg(x)	((x)*180./PI)
#define	hrdeg(x)	((x)*15.)
#define	deghr(x)	((x)/15.)
#define	hrrad(x)	degrad(hrdeg(x))
#define	radhr(x)	deghr(raddeg(x))

#define	DCOS(x)		cos(degrad(x))
#define	DSIN(x)		sin(degrad(x))
#define	DASIN(x)	raddeg(asin(x))
#define	DATAN2(y,x)	raddeg(atan2((y),(x)))

/* ratio of from synodic (solar) to sidereal (stellar) rate */
#define	SIDRATE		.9972695677

#define	SPD	(24.0*3600.0)	/* seconds per day */

#define NUT_SCALE	1e4
#define NUT_SERIES	106
#define NUT_MAXMUL	4
#define SECPERCIRC	(3600.*360.)


/* manifest names for planets.
 * N.B. must coincide with usage in pelement.c and plans.c.
 * N.B. only the first 8 are valid for use with plans().
 */
#define	MERCURY	0
#define	VENUS	1
#define	MARS	2
#define	JUPITER	3
#define	SATURN	4
#define	URANUS	5
#define	NEPTUNE	6
#define	PLUTO	7

/* a few more handy ones */
#define	SUN	8
#define	MOON	9
#define	OBJX	10
#define	OBJY	11
#define	OBJZ	12
#define	NOBJ	13	/* total number of basic objects */

#define M_PI            3.14159265358979323846
/* starting point for MJD calculations
 */
#define MJD0  2415020.0
#define J2000 (2451545.0 - MJD0)      /* let compiler optimise */

#define VSOP_ASCALE	1e8	/* amplitude factor as stored */
#define ABERR_CONST	(20.49552/3600./180.*PI)  /* aberr const in rad */
/* global function declarations */
  void ma(double lat, double ha, double dec, double *alt,
    double *az, double *fpa);
/* aa_hadec.c */
  void aa_hadec(double lat, double alt, double az, double *ha,
    double *dec);
  void hadec_aa(double lat, double ha, double dec, double *alt,
    double *az); 

/* aberration.c 
  void ab_ecl P_((double mjd, double lsn, double *lam, double *bet));
  void ab_eq P_((double mjd, double lsn, double *ra, double *dec));
*/
void ab_eq(double mjd, double lsn, double *ra, double *dec);
void now_lst(double mjd0_,double lng_, double *lstp);
/* airmass.c 
  void airmass P_((double aa, double *Xp));
*/
/* anomaly.c 
  void anomaly P_((double ma, double s, double *nu, double *ea));
*/
/* chap95.c 
  int chap95 P_((double mjd, int obj, double prec, double *ret));
*/
/* chap95_data.c */

/* comet.c 
  void comet P_((double mjd, double ep, double inc, double ap, double qp,
    double om, double *lpd, double *psi, double *rp, double *rho, double *lam,
    double *bet));
*/
/* deltat.c 
  double deltat P_((double mjd));
*/
/* eq_ecl.c 
  void eq_ecl P_((double mjd, double ra, double dec, double *lat,
    double *lng));
  void ecl_eq P_((double mjd, double lat, double lng, double *ra,
    double *dec));
*/
/* eq_ecl_p03.c 
  void eq_ecl_p03 P_((double mjd, double ra, double dec, double *lat,
    double *lng));
  void ecl_eq_p03 P_((double mjd, double lat, double lng, double *ra,
    double *dec));
  void obliquity_p03 P_((double mjd, double *eps));
*/
  void obliquity_p03(double mjd, double *eps);
/* eq_gal.c 
  void eq_gal P_((double mjd, double ra, double dec, double *lat,
    double *lng));
  void gal_eq P_((double mjd, double lat, double lng, double *ra,
    double *dec));
*/
/* formats.c 
  void fs_sexa P_((char *out, double a, int w, int fracbase));
  void fs_date P_((char out[], double jd));
  void f_scansex P_((double o, char *str, double *dp));
  void f_sscandate P_((char *bp, int pref, int *m, double *d, int *y));
  int scansex P_((char *str, double *dp));
*/
/* helio.c 
  void heliocorr P_((double jd, double ra, double dec, double *hcp));
*/
/* libration.c 
extern void llibration P_((double JD, double *llatp, double *llonp));
*/
/* misc.c 
extern void zero_mem P_((void *loc, unsigned len));
extern int tickmarks P_((double min, double max, int numdiv, double ticks[]));
extern int lc P_((int cx, int cy, int cw, int x1, int y1, int x2, int y2,
    int *sx1, int *sy1, int *sx2, int *sy2));
extern void hg_mag P_((double h, double g, double rp, double rho, double rsn,
    double *mp));
extern int magdiam P_((int fmag, int magstp, double scale, double mag,
    double size));
extern void gk_mag P_((double g, double k, double rp, double rho, double *mp));
extern double atod P_((char *buf));
extern void solve_sphere P_((double A, double b, double cc, double sc,
    double *cap, double *Bp));
extern double delra P_((double dra));
*/
  void solve_sphere(double A, double b, double cc, double sc,
    double *cap, double *Bp);
/* mjd.c 
extern void cal_mjd P_((int mn, double dy, int yr, double *mjd));
extern void mjd_cal P_((double mjd, int *mn, double *dy, int *yr));
extern int mjd_dow P_((double mjd, int *dow));
extern int isleapyear P_((int year));
extern void mjd_dpm P_((double mjd, int *ndays));
extern void mjd_year P_((double mjd, double *yr));
extern void year_mjd P_((double y, double *mjd));
extern void rnd_second P_((double *t));
extern void mjd_dayno P_((double jd, int *yr, double *dy));
extern double mjd_day P_((double jd));
extern double mjd_hr P_((double jd));
extern void range P_((double *v, double r));
*/
void mjd_cal(double mjd, int *mn, double *dy, int *yr);
void cal_mjd(int mn, double dy, int yr, double *mjd_);
double deltat(double mjd);
void mjd_year(double mjd, double *yr);
void range(double *v, double r);
/* moon.c 
extern void moon P_((double mjd, double *lam, double *bet, double *rho,
    double *msp, double *mdp));
*/
/* mooncolong.c 
extern void moon_colong P_((double jd, double lt, double lg, double *cp, double *kp, double *ap, double *sp));
*/
/* nutation.c */
  void nutation(double mjd, double *deps, double *dpsi);
  void nut_eq(double mjd, double *ra, double *dec);

/* obliq.c 
extern void obliquity P_((double mjd, double *eps));
*/
/* parallax.c 
extern void ta_par P_((double tha, double tdec, double phi, double ht,
    double *rho, double *aha, double *adec));
*/
/* plans.c 
extern void plans P_((double mjd, int p, double *lpd0, double *psi0,
    double *rp0, double *rho0, double *lam, double *bet, double *dia,
    double *mag));
*/
/* precess.c 
extern void precess P_((double mjd1, double mjd2, double *ra, double *dec));
*/
/* precess_p03.c */
  void precess_p03(double mjd1, double mjd2, double *ra, double *dec);

/* reduce.c 
extern void reduce_elements P_((double mjd0, double mjd, double inc0,
    double ap0, double om0, double *inc, double *ap, double *om));
*/
/* refract.c */
  void unrefract(double pr, double tr, double aa, double *ta);
  void refract(double pr, double tr, double ta, double *aa);
  void unrefractLT15(double pr, double tr, double aa, double *ta);
  void unrefractGE15(double pr, double tr, double aa, double *ta);
/* rings.c 
extern void satrings P_((double sb, double sl, double sr, double el, double er,
    double JD, double *etiltp, double *stiltp));
*/
/* riset.c 
extern void riset P_((double ra, double dec, double lat, double dis,
    double *lstr, double *lsts, double *azr, double *azs, int *status));
*/
/* sphcart.c*/ 
  void sphcart(double l, double b, double r, double *x, double *y,
    double *z);
  void cartsph(double x, double y, double z, double *l, double *b,
    double *r);

/* sun.c */
  void sunpos(double mjd, double *lsn, double *rsn, double *bsn);

/* twobody.c 
extern void vrc P_((double *v, double *r, double tp, double e, double q));
*/
/* utc_gst.c 
extern void utc_gst P_((double mjd, double utc, double *gst));
extern void gst_utc P_((double mjd, double gst, double *utc));
*/
/* vsop87.c 
extern int vsop87 P_((double mjd, int obj, double prec, double *ret));
*/
/* For RCS Only -- Do Not Edit
 * @(#) $RCSfile: astro.h,v $ $Date: 2001/10/16 21:45:05 $ $Revision: 1.5 $ $Name:  $
 */
