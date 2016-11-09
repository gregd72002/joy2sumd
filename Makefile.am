bin_PROGRAMS = joy2sumd 
joy2sumd_SOURCES = main.c sumd.c convert_ps3.c
joy2sumd_CFLAGS = -Wall
joy2sumd_LDFLAGS = 
joy2sumd_LDADD = -lm

joy2sumdbindir=$(bindir)

install-exec-hook:
	cp utils/joy2sumd.sh $(joy2sumdbindir)/
	chmod 777 $(joy2sumdbindir)/joy2sumd.sh

uinstall-hook:
	rm $(joy2sumdbindir)/joy2sumd.sh

