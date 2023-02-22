
MPIR=0

#RT=RT_AAA
#OD=$(RT)/bt_$(PLAT)/

RT=../e3elems
#OD=$(RT)/bt_$(PLAT)/

include plat.mak


#INCS = -I$(RT) -I. -DE3NOABORT=1
#HEAD =

#ifneq ($(MPIR),0)
#INCS += -I$(RT)/$(MPIRD0)
#HEAD += $(RT)/$(MPIRD0)/mpir.h
#endif

include mk_opts.mak

include plat_$(PLAT).mak

src1 = main.cpp poly.cpp

obj1 := $(src1:%.cpp=o/%.$(OEXT))

include mk_rules.mak

