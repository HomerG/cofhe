#include <iostream>
#include <gmp.h>
#include <string>

using namespace std;

int main()
{
	//"1", "288794978602139552", "178930308976060547", "748001537669050592" "632424960050479415", "863392794822869617", "320225457230408771", "836916420051356794", "722043138442168580"
	mpz_t twiddle_coeff, mod, r;
	mpz_init_set_str(twiddle_coeff, "288794978602139552", 10);
	mpz_init_set_str(mod, "1152921504606584833", 10);
	mpz_powm_ui(r, twiddle_coeff, 4, mod);
	gmp_printf("%Zd\n", r);
}

