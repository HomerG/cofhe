print ("INFO: Running Regression for Cryptoleq")
for x in range(0, 3):
  arga             = random.randint(0,ccs0201_init_val.MOD)
  argb             = random.randint(0,ccs0201_init_val.MOD)
  mul_val          = ccs0201_tasks.mod_mul (arga, argb)
  exp_val          = ccs0201_tasks.mod_exp (arga, argb)
  inv_err, inv_val = ccs0201_tasks.mod_inv (arga, ccs0201_init_val.MOD)
  gfunc_res        = ccs0201_tasks.gfunc (arga, argb)


