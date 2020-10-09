typedef struct CComp CComp;

enum {
	Nmaker = 16,
	Nmodel = 32,
	Ncpu = 8,
	Ndesc = 192,
};

struct CComp {
	char maker[Nmaker];
	char model[Nmodel];
	int year;
	char cpu[Ncpu];
	int id;
	char desc[Ndesc];
};
