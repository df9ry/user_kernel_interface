#   Project user_kernel_interface
#   Copyright (C) 2019 tania@df9ry.de
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Affero General Public License as
#   published by the Free Software Foundation, either version 3 of the
#   License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Affero General Public License for more details.
#
#   You should have received a copy of the GNU Affero General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
ifeq (,$(filter _%,$(notdir $(CURDIR))))
include target.mk
else
#----- End Boilerplate

VPATH = $(SRCDIR)

CFLAGS   =  -Wall -g -fmessage-length=0 -pthread

LDFLAGS  =  -Wall -g -fmessage-length=0 -pthread
			
OBJS     =  \
	user_kernel_interface.o \
	list_test.o \
	container_test.o
			
LIBS     =  -lpthread

TARGET   =  user_kernel_interface

$(TARGET):  $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	
%.o: %.c uki/*.h
	$(CC) $(CFLAGS) -c $<	
	
all: $(TARGET)
	echo "Build OK"

doc: $(DOCDIR)
	doxygen ../doxygen.conf
	( cd ../_doc/latex && make )

test: $(TARGET)
	./$(TARGET)
	
install:
	sudo cp $(TARGET) /usr/local/bin
	sudo chown root:staff /usr/local/bin/$(TARGET)
	sudo cp -rf ../uki /usr/local/include
	sudo chown -R root:staff /usr/local/include/uki
	
	
#----- Begin Boilerplate
endif
