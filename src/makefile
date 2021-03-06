# set the C compiler, for C++ use g++
# use this setting on most sane machines
CC = gcc

# use this line on the engin linux machines
# CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =../lib

# path to the bin directory
BINDIR =../bin

# libraries to include
LIBS = -lm -limageIO
LFLAGS = -L$(LIBDIR)

# put all of the relevant include files here
_DEPS = ppmIO.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = ppmtest

# put a list of all the object files here for all executables (with .o endings)
_OBJ = ppmmain.o

# convert them to point to the right place
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# patterns for compiling source code
# $< is the file that caused the action to occur
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all:
	for i in $(EXECUTABLES); do (make $$i) ; done


# put the specific executable rules here
ppmtest: $(ODIR)/ppmmain.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

lab1: $(ODIR)/lab1.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

lab1mask: $(ODIR)/lab1mask.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

lab1alpha: $(ODIR)/lab1alpha.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

lab1transform: $(ODIR)/lab1transform.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

imagetest: $(ODIR)/imagetest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

fractaltest: $(ODIR)/fractaltest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

texturetest: $(ODIR)/texturetest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

testbench: $(ODIR)/testbench.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

circletest: $(ODIR)/circletest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

ellipsetest: $(ODIR)/ellipsetest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

testProg1: $(ODIR)/testProg1.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) 

testProg2: $(ODIR)/testProg2.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) 

testProg3: $(ODIR)/testProg3.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) 

main: $(ODIR)/main.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) 

lineTestSmall: $(ODIR)/lineTestSmall.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) 

lego: $(ODIR)/lego.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) 

test4a: $(ODIR)/test4a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS) 

test4b: $(ODIR)/test4b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
 
test4c-loop: $(ODIR)/test4c-loop.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

rabbit: $(ODIR)/rabbit.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

alpaca: $(ODIR)/alpaca.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

car: $(ODIR)/car.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

legoMan: $(ODIR)/legoMan.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5a: $(ODIR)/test5a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5b: $(ODIR)/test5b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5c: $(ODIR)/test5c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

crossTest: $(ODIR)/crossTest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

2dscene: $(ODIR)/2dscene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

3Dscene: $(ODIR)/3Dscene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6a: $(ODIR)/test6a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

blueCube: $(ODIR)/blueCube.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6b: $(ODIR)/test6b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

pyramid: $(ODIR)/pyramid.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

mountain: $(ODIR)/mountain.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

legoMod: $(ODIR)/legoMod.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7a: $(ODIR)/test7a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7b: $(ODIR)/test7b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7c: $(ODIR)/test7c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

teapot: $(ODIR)/teapot.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

bezierWave: $(ODIR)/bezierWave.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

raindrops: $(ODIR)/raindrops.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test8a: $(ODIR)/test8a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

cubism: $(ODIR)/cubism.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

cubemove: $(ODIR)/cubemove.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

testLighting_shading: $(ODIR)/testLighting_shading.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test9d: $(ODIR)/test9d.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test9a: $(ODIR)/test9a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test9b: $(ODIR)/test9b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
	
test9c: $(ODIR)/test9c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
	
test9e: $(ODIR)/test9e.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

torch: $(ODIR)/torch.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

