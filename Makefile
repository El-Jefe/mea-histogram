PLUGIN_NAME = mea_histogram

HEADERS = mea-histogram.h

SOURCES = mea-histogram.cpp\
          moc_mea-histogram.cpp\

LIBS = -lqwt 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
