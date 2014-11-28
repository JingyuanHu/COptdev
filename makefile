# makefile for optimization framework
# written by Ruimin Wang
# powered by 'MathU'
# copyright@MathU

<<<<<<< HEAD
DIR_INC += -IE:/libs/eigen -I./include/Base -I./include/LeastSquares -I./include/FunctionRepository -I./include/Algorithms -I./include/ThirdParty -I./include
DIR_SRC += ./src/Frame ./src/LeastSquares ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin
DIR_LIB = -LE:/libs
=======
include ./Makefile.in
>>>>>>> upstream/master




#objects = test.o basicmath.o

# INCLUDE += -I/usr/local/Cellar/Eigen/3.2.1/include/eigen3 -Iinclude/Frame/

# vpath %.cpp include/Frame

all:$(BIN_TARGET)


$(BIN_TARGET):$(OBJ)
	$(CC) $(OBJ) $(DIR_LIB) -o $@ -lcblas -lblas

$(DIR_OBJ)/%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	find $(DIR_OBJ) -name *.o -exec rm -rf {} \;


# all debug
# all:
# 	@echo $(BIN_TARGET)
# 	@echo $(SRC)
# 	@echo $(notdir $(SRC))
# 	@echo $(OBJ)
# 	@echo $(TT)
# 	@echo "end"




# test for matrix
matrix: bin/matrix
bin/matrix: obj/matrix.o
	$(CC) obj/matrix.o $(DIR_LIB) -o $@ -lcblas -lblas
obj/matrix.o: test/matrix.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# test for simplex method
simplex: bin/simplex
bin/simplex: obj/simplex.o
	$(CC) obj/simplex.o $(DIR_LIB) -o $@ -lcblas -lblas
obj/simplex.o: test/simplex.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# test for matrix vector
vecmat: bin/vecmat
bin/vecmat: obj/vecmat.o
	$(CC) obj/vecmat.o $(DIR_LIB) -o $@ -lcblas -lblas
obj/vecmat.o: test/matrix_vector.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# test for sparse matrix
spmat: bin/spmat
bin/spmat: obj/spmat.o
	$(CC) obj/spmat.o $(DIR_LIB) -o $@ -lcblas -lblas
obj/spmat.o:test/spmat.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# test for umfpack
umfpack: bin/umfpack
bin/umfpack: obj/umfpack.o
	$(CC) obj/umfpack.o $(DIR_LIB) -o $@ -lcblas -lblas -lumfpack -lamd -lsuitesparseconfig -lcholmod -lcolamd
obj/umfpack.o:test/umfpack.cpp
	$(CC) $(CFLAGS) -c $< -o $@

umfpackwrapper: bin/umfpackwrapper
bin/umfpackwrapper: obj/umfpackwrapper.o
	$(CC) obj/umfpackwrapper.o $(DIR_LIB) -o $@ -lcblas -lblas -lumfpack -lamd -lsuitesparseconfig -lcholmod -lcolamd
obj/umfpackwrapper.o: test/umfpack_wrapper.cpp
	$(CC) $(CFLAGS) -c $< -o $@

omp: bin/omp
bin/omp: obj/omp.o
	$(CC) obj/omp.o $(DIR_LIB) -o $@ -lcblas -lblas -lumfpack -lamd -lsuitesparseconfig -lcholmod -lcolamd
obj/omp.o: test/omp.cpp
	$(CC) $(CFLAGS) -c $< -o $@
#$(TEST_BIN): $(TEST_OBJ)
#	$(CC) $(TEST_OBJ) $(DIR_LIB) -o $@ -lcblas -lblas

#$(TEST_OBJ):$(TEST_SRC)
#	$(CC) $(CFLAGS) -c $< -o $@

help: $(TEST_BIN) 
	
$(TEST_BIN): $(TEST_OBJ)
	$(CC) $(TEST_OBJ) $(DIR_LIB) -o $@ -lcblas -lblas
$(TEST_OBJ):$(TEST_SRC)
	$(CC) $(CFLAGS) -c $< -o $@



#test : $(objects)
#	$(CC) -o test $(objects)

#test.o : include/Frame/test.cpp
#	$(CC) -c $(INCLUDE) include/Frame/test.cpp

#basicmath.o : include/Frame/basicmath.cpp include/Frame/basicmath.h
#	$(CC) -c  $(INCLUDE) include/Frame/basicmath.cpp

#.PHONY : clean
#clean :
#	-rm test $(objects)