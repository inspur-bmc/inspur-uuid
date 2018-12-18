sbin_PROGRAMS = inspur-uuid

inspur_uuid_SOURCES = \
	controller.cpp  \
	gpio.cpp  \
	physical.cpp

inspur_uuid_LDFLAGS = \
		      -lgpioplus \
		      $(GPIOPLUS_LIBS)\
		      $(SDBUSPLUS_LIBS) \
		      $(SDEVENTPLUS_LIBS) \
                      $(PHOSPHOR_DBUS_INTERFACES_LIBS) 

inspur_uuid_CFLAGS =  $(SDBUSPLUS_CFLAGS) \
                      $(PHOSPHOR_DBUS_INTERFACES_CFLAGS) \
		      $(SDEVENTPLUS_CFLAGS) \
		      $(GPIOPLUS_CFLAGS)
