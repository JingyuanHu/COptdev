#include <Header>

typedef double 		FT;
typedef COPT::VectorBase<FT>	Vector;
typedef COPT::MatrixBase<FT> 	Matrix;
typedef COPT::KernelTrait<FT> 	kernel;
typedef COPT:: SimplexSolver<kernel>	Simplex;

int main(int argc,char* argv[])
{
	// Vector c(4);
	// c[0] = -3;
	// c[1] = -2;
	// c[2] = 0;
	// c[3] = 0;
	// Matrix A(2,4);
	// A(0,0)=1;
	// A(0,1)=1;
	// A(0,2)=1;
	// A(1,0)=2;
	// A(1,1)=0.5;
	// A(1,3)=1;
	// Vector b(2),x;   
	// b[0]=5;
	// b[1]=8;
	Vector c(5);
	c[0]=-4;
	c[1]=-1;
	Matrix A(3,5);
	A(0,0)=-1;A(0,1)=2.0;A(0,2)=1.0;
	A(1,0)=2.0;A(1,1)=3.0;A(1,3)=1.0;
	A(2,0)=1.0;A(2,1)=-1.0;A(2,4)=1.0;
	Vector b(3);
	b(0)=4.0;b(1)=12.0;b(2)=3.0;
	// std::vector<size_t> indb,indn,indz;
	// indb.push_back(2);
	// indb.push_back(3);
	// indn.push_back(0); 
	// indn.push_back (1);   
	// Simplex::findFeasiblePoint(2,4,A,b,indb,indn,indz);
	// std::cout<<indb.size()<<std::endl;
	// std::cout<<indn.size()<<std::endl;
	// std::cout<<indz.size()<<std::endl;
	// Simplex::simplexMethodWithIndices(2,4,A,b,c,x,indb,indn,indz);
	// std::cout<<x<<std::endl;
	Simplex solver(A,b,c);
	std::cout<<solver.stringSolverStatus()<<std::endl; 
	std::cout<<solver.solve()<<std::endl;
	std::cout<<solver.stringSolverStatus()<<std::endl;
	std::cout<<solver.result()<<std::endl;
	std::cout<<solver.val()<<std::endl;

	// std::cout<<"current status is "<<Simplex::oneSimplexStep(A,b,c,indb,indn);
	// std::cout<<"current status is "<<Simplex::oneSimplexStep(A,b,c,indb,indn);
	// std::cout<<"current status is "<<Simplex::oneSimplexStep(A,b,c,indb,indn);
}