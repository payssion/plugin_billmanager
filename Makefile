MGR = billmgr
PLUGIN = pmpayssion

CFLAGS += -I/usr/local/mgr5/include/billmgr
CXXFLAGS += -I/usr/local/mgr5/include/billmgr

PKGNAMES = billmanager-plugin-pmpayssion
RPM_PKGNAMES ?= $(PKGNAMES)
DEB_PKGNAMES ?= $(PKGNAMES)

WRAPPER += pmpayssion payssionpayment payssionresult

pmpayssion_SOURCES = pmpayssion.cpp
pmpayssion_LDADD = -lbase -lpaymentmodule
pmpayssion_FOLDER = paymethods

payssionpayment_SOURCES = payssionpayment.cpp
payssionpayment_LDADD = -lpaymentcgi
payssionpayment_FOLDER = cgi

payssionresult_SOURCES = payssionresult.cpp
payssionresult_LDADD = -lpaymentcgi
payssionresult_FOLDER = cgi

BASE ?= /usr/local/mgr5
include $(BASE)/src/isp.mk
