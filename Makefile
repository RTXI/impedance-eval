PLUGIN_NAME = impedance_eval

HEADERS = impedance-eval.h

SOURCES = impedance-eval.cpp\
          moc_impedance-eval.cpp\

LIBS = 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
