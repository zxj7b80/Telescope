class TimeAndCoordinate
{
public:
	/**
	 *ܣJ2000ԪµĳྭγתǰԪ³ྭγ
	 *
	 *inRA     - ྭ٣Ƕ
	 *inDEC    - γ٣Ƕ
	 *outEpoch - 뵱ǰԪ٣ԼգMJD
	 *outRA    - ྭ٣Ƕ
	 *outDEC   - γ٣Ƕ
	 *****************************************************************************************
	 *	inRA - input Right ascension(decimal degree)
	 *	inDEC - input Declination(decimal degree)
	 *	outEpoch - output Epoch(real epoch)
	 *	outRA - output Right ascension(decimal degree)
	 *	outDEC - output Declination(decimal degree)
	 *Return:void
	 *NotesConvert the Right ascension and Declination from J2000 to real epoch
	 **/                         

	void Epoch2Actual(double inRA,double inDEC,double outEpoch,double &outRA, double &outDEC);


	/**
	 *ܣǰԪµĳྭγתJ2000Ԫ³ྭγ
	 *inEpoch - 뵱ǰԪ٣Լ
	 *inRA    - ྭ٣Ƕ
	 *inDEC   - γ٣Ƕ
	 *outRA   - ྭ٣Ƕ
	 *outDEC  - γ٣Ƕ
	 *****************************************************************************************
	 *	inEpoch - input Epoch(real epoch)
	 *	inRA - input Right ascension(decimal degree)
	 *	inDEC - input Declination(decimal degree)
	 *	outRA - output Right ascension(decimal degree)
	 *	outDEC - output Declination(decimal degree)
	 *Return: void
	 *Notes: Convert the Right ascension and Declination from real epoch to J2000
    	 **/
	void Actual2Epoch(const double inEpoch, double inRA, double inDEC,double& outRA, double& outDEC) ;

    
	/**
	 *ܣʱʽתΪԼԼձʾԪ
	 *Year     - ݣUTCʱ
	 *Month    - ·ݣUTCʱ
	 *Day      C UTCʱ
	 *Hour     - СʱUTCʱ
	 *Minute   - ӣUTCʱ
	 *Second   - 룬UTCʱ
	 *MicroSec - ΢룬UTCʱ
	 *ֵEpoch   - ԼգUTCʱ	
	 *****************************************************************************************
	 *	Year - Year, UTC(coordinated universal time)
	 *	Month - Month, UTC(coordinated universal time)
	 *	Day - Day, UTC(coordinated universal time)
	 *	Hour - Hour, UTC(coordinated universal time)
	 *	Minute - Minute, UTC(coordinated universal time)
	 *	Second - Second, UTC(coordinated universal time)
	 *	MicroSec - Microsecond, UTC(coordinated universal time)
	 *Returnmodified Julian date
	 **/
    double GetEpoch(int Year,int Month,int Day,int Hour,int Minute,int Second,int MicroSec);


	
	/** 
	 *ܣȡγȵصĵطʱ
	 *Lgt      - ȣ٣ǶȣΪ
	 *Lat      - γȣ٣ǶȣγΪ
	 *Year     - ݣUTCʱ
	 *Month    - ·ݣUTCʱ
	 *Day      - UTCʱ
	 *Hour     - СʱUTCʱ
	 *Minute   - ӣUTCʱ
	 *Second   - 룬UTCʱ
	 *MicroSec - ΢룬UTCʱ
	 *ֵ: LMST   - طʱ,(Ƕ)
	 *****************************************************************************************
	 * 	Lgt - longitude (decimal degree, east longitude is positive)
	 *  	Lat - latitude (decimal degree, northern latitude is positive)
	 *	Year - Year, UTC(coordinated universal time)
	 *	Month - Month, UTC(coordinated universal time)
	 *	Day - Day, UTC(coordinated universal time)
	 *	Hour - Hour, UTC(coordinated universal time)
	 *	Minute - Minute, UTC(coordinated universal time)
	 *	Second - Second, UTC(coordinated universal time)
	 *	MicroSec - Microsecond, UTC(coordinated universal time)
	 *Returnlocal sidereal time (decimal degree)
	 **/
    double GetLMST(double Lgt,double Lat,int Year,int Month,int Day,int Hour,int Minute,int Second,int MicroSec);


	/** 
	 *ܣվƽϵ·λǡ߶Ƚתϵ³ྭγ
	 *Azi  - 뷽λǣ٣Ƕȣ
	 *Alt  - 븩ǣ٣Ƕȣˮƽ
	 *LMST - 뱾غʱ٣Ƕ
	 *Lgt  - ȣ٣ǶȣΪ
	 *Lat  - γȣ٣ǶȣγΪ
	 *RA   - ྭ٣ǶȣϵǰԪ
	 *DEC  - γ٣ǶȣϵǰԪ
	 *****************************************************************************************
	 *	Azi - input Azimuth (decimal degree, North zero)
	 *	Alt - input Altitude (decimal degree, horizon zero)
	 *	LMST - input Local Mean sidereal time (decimal degree)
	 *	Lgt - input longitude(decimal degree, east longitude is positive)
	 *	Lat - input latitude(decimal degree, northern latitude is positive)
	 *	RA - output Right ascension(decimal degree,(Coordinate system: real epoch)
	 *	DEC - output Declination(decimal degree Coordinate system: real epoch)
	 *Returnvoid
	 *notesTransfer function from horizontal coordinate system to equatorial coordinate system.
	 **/
    void AltAzi2Eq(double Azi, double Alt, double LMST, double Lgt, double Lat, double &RA, double &DEC);


	
	/**
	 *ܣվϵ³ྭγתƽϵ·λǡ߶Ƚ,ͬʱݽ͵ѡȷת
	 *RA    - ྭ٣ǶȣϵǰԪ
	 *DEC   - γ٣ǶȣϵǰԪ
	 *LMST  - 뱾غʱ٣Ƕ
	 *Lgt   - ȣ٣ǶȣΪ
	 *Lat   - γȣ٣ǶȣγΪ
	 *Focus - 뽹͡12ͽ3ͽ
	 *Azi   - λǣ٣Ƕȣ
	 *Alt   - ǣ٣Ƕȣˮƽ
	 *Rot   - תǣ٣Ƕ
	 *****************************************************************************************
	 *	RA - input Right ascension(decimal degree,(Coordinate system: real epoch)
	 *	DEC - input Declination(decimal degree Coordinate system: real epoch)
	 *	LMST - input Local Mean sidereal time (decimal degree)
	 *	Lgt - input longitude(decimal degree, east longitude is positive)
	 *	Lat - input latitude(decimal degree, northern latitude is positive)
	 *	Focus - input Focus type. 1: Cassegrain focus; 2: East Nasmyth focus 3: West Nasmyth focus
	 *	Azi - output Azimuth (decimal degree, North zero)
	 *	Alt - output Altitude (decimal degree, horizon zero)
	 *	Rot - output Rotation angle of image field(dimension: decimal degree)
	 *Returnvoid
	 *NotesTransfer function from equatorial coordinate system to horizontal coordinate system.
     **/
    void Eq2AltAzi(double RA,double DEC,double LMST,double Lgt,double Lat,int Focus,double &Azi,double &Alt,double &Rot);
};

