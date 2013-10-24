all:
	$(MAKE) -C lib -f make.boost.mk clean
	$(MAKE) -C lib -f make.boost.mk
	$(MAKE) -f make.mk clean
	$(MAKE) -f make.mk
