#ifndef BSCSEARCH_H
#define BSCSEARCH_H



class BSCSearch
{
public:
    BSCSearch(void);
public:
    struct oneStar{
        int          HRnum;
        char         starname[11];
        char         spytype[20];
        int          rah;          //uint hour
        int          ram;          //uint minute
        double       ras;          //uint second
        int          decd;         //uint deg
        int          decm;         //uint arcm
        int          decs;         //uint arcs
        double       magV;
        double       magB;
        double       ra;           //uint hour
        double       dec;          //uint deg
        oneStar      *LLINK;
        oneStar      *RLINK;
    };
public:
    bool SetStellarCatalog (const char *pszPath);
    bool SelStarByPos (const char *pszPath, double RA, double DEC, oneStar *oneStar);
    int SelStars (const char *pszPath, double RA, double DEC, double Field, oneStar **starList);
    bool FindStarByName(const char *pszName, oneStar &oneStar);
    void AsteroidRaDec(const char *pszPath, int Year, int Month, int Day, int Hour, int Minute, oneStar *oneStar);
};

#endif // BSCSEARCH_H
