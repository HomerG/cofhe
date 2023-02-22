
int cofhe_init(unsigned char *portname);

int fhe_ntt (int poly_degree, int poly_width, unsigned char * mod, unsigned char * in, unsigned char *twdl, unsigned char *out);

int fhe_poly_mul (int poly_degree, int poly_width, unsigned char * mod, unsigned char * in1, unsigned char *in2, unsigned char *twdl, unsigned char *out);

