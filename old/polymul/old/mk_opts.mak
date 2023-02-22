
INCS = -I$(RT) -I. -DE3NOABORT=1 -I$(RT)/$(MPN)
#INCS = -I$(RT) -I. -DE3NOABORT=1 -I$(RT)/$(MPN) -DEDUINT
HEAD =

ifneq ($(MPIR),0)
INCS += -I$(RT)/$(MPIRD0)
HEAD += $(RT)/$(MPIRD0)/mpir.h
endif

