#ifndef NON_LINEAR_SQUARE_H
#define NON_LINEAR_SQUARE_H


namespace COPT
{

template<class Problem, class Time = NoTimeStatistics>
class NonLinearSquare
	:
	public GeneralSolver<typename Problem::KernelTrait, Time>
{
private:
	typedef typename Problem::KernelTrait::index		index;
	typedef typename Problem::KernelTrait::scalar 		scalar;
	typedef typename Problem::KernelTrait::Vector 		Vector;
	typedef typename Problem::KernelTrait::Matrix 		Matrix;

	/*
	 *		vector functions that are used
	 *				
	 */

	 const Problem&				__p;
	//	point value
	Vector						__x;
	//	function dimension
	index 						__m;
	//	variable dimension
	index						__n;
	//	constants
	scalar 						__tao;
	scalar						__v;
	//	iteration number
	index 						__iter_num;
	index						__maxiteration;
	/*	A is approximately equal to Hessian	matrix 	*/	
	Matrix 						__A;	
	/*	g is approximately equal to Gradient	*/
	Vector 						__g;
	//	scaling factor
	scalar						__u;
	bool						__found;


	/*
	 *		non-linear square solver
	 */

	void solvingBegin();
	void doCompute();
	scalar doOneIteration();
	void doSolve();
public:
	/*
	 *		Constructor
	 */ 
	NonLinearSquare(
		const Problem& p,
		const Vector& x,
		const scalar tao = 1e-3,
		const scalar v = 2.0,
		const index iternum = 1000
		);
	
	// 	output
	void printInfo();
	//	objective value
	scalar objective() const;
	// 	optimal point
	const Vector& result() const;

};

template<class Problem,class Time>
void NonLinearSquare<Problem,Time>::doCompute( )
{
	__A = __p.Jacobi(__x).transpose()*(__p.Jacobi(__x));	
	__g = __p.Jacobi(__x)*(__p.Fvalue(__x));

	scalar max = fabs(__A(0,0));
	for(int i=0; i<__n; i++){
		if(fabs(__A(i,i))>max)
			max=__A(i,i);
	}
	__u = __tao*max;
}

template<class Problem,class Time>
NonLinearSquare<Problem,Time>::NonLinearSquare( 
	const Problem& p,
	const Vector& x,
	const scalar tao,
	const scalar v,
	const index iternum
	)
    	:
    	__p(p),
    	__x(x),
    	__tao(tao),
    	__v(v),
    	__maxiteration(iternum)
{

	this->doCompute();
}

template<class Problem,class Time>
void NonLinearSquare<Problem,Time>::solvingBegin()
{
	__m = __p.functiondimension();
	__n = __p.variabledimension();
}

template<class Problem,class Time>
typename NonLinearSquare<Problem,Time>::scalar  NonLinearSquare<Problem,Time>::doOneIteration()
{
	/*
		Levenberg Marquardt method
	*/

	scalar e = 1e-8;
	scalar rho;
	Vector h;
	Vector x;

	__found = (__g.dot(__g) <= e);
	Matrix B(__A);
	Vector k(__g);	

	for(int i=0; i<__n; i++){
		B(i,i) = __A(i,i)+__u;
		k[i]=-__g[i];
	}
		
	/*	Solve linear equations	*/	
	h = B.solve(k);

	if ( h.dot(h) < e*(e+__x.dot(__x)) )
	{
		__found = true;
		return h.dot(h);
	}
	else
	{
		x = __x + h;

		/*	Judgment flag	rho = (F(__x)-F(x))/L(h,u,g)	*/
		rho = (__p.Fvalue(__x).dot(__p.Fvalue(__x))/2-__p.Fvalue(x).dot(__p.Fvalue(x))/2)/(0.5*h.dot(__u*h-__g));

		if (rho > 0.0)
		{
			__x = x;
			__A = __p.Jacobi(__x).transpose()*(__p.Jacobi(__x));
			__g = __p.Jacobi(__x).transpose()*(__p.Fvalue(__x));
			__found = ( __g.dot(__g) <= e );
			scalar uu=1/3;
			if ( 1-(2*rho-1)*(2*rho-1)*(2*rho-1) > uu )
			{
				uu = 1-(2*rho-1)*(2*rho-1)*(2*rho-1);
			}
			__u = __u*uu;
			__v = 2.0;
		}
		else
		{
			__u = __u*__v;
			__v = 2*__v;
		}	
		return __g.dot(__g);		
	}
}

template<class Problem,class Time>
void NonLinearSquare<Problem,Time>::doSolve()
{
	__iter_num = 0;
	do
	{
		this->__estimated_error = this->oneIteration();
		if(++__iter_num >= __maxiteration)
		{
			this->__terminal_type = this->MaxIteration;
			break;
		}
	}while( !__found );
	if( __found )
		this->__terminal_type = this->Optimal;
}

template<class Problem,class Time>
void NonLinearSquare<Problem,Time>::printInfo()
{
	std::cout<<"The optimal point is "<<__x<<std::endl;
}

template<class Problem,class Time>
typename NonLinearSquare<Problem,Time>::scalar NonLinearSquare<Problem,Time>::objective() const
{
	return __p.objective(__x);
}

template<class Problem,class Time>
const typename NonLinearSquare<Problem,Time>::Vector& NonLinearSquare<Problem,Time>::result() const
{
	return __x;
}


/***********************Implementation of NonLinearSquareProblem************************/

template<class kernel>
class NonLinearSquareProblem
	:
	public VectorProblem<kernel>
{
private:
	typedef typename kernel::index			index;
	typedef typename kernel::scalar 		scalar;
	typedef typename kernel::Matrix 		Matrix;
	typedef typename kernel::Vector 		Vector;

	VectorFunctionSystem<scalar>		__vfs;
	index 					__m;
	index 					__n;
	scalar					__lambda;

public:
	NonLinearSquareProblem(
		const VectorFunctionSystem<scalar>& vfs,
		const index m,
		const index n,
		const scalar lambda = 0.5);

	bool isValidInput (const Vector& x) const;
	bool isValid() const;
	const index& functiondimension() const;
	const index& variabledimension() const;
	const scalar& lambda() const;
	Vector Fvalue(const Vector& x) const;
	Matrix Jacobi(const Vector& x) const;
	scalar objective( const Vector& x) const;

};


template<class kernel>
NonLinearSquareProblem<kernel>::NonLinearSquareProblem(
	const VectorFunctionSystem<scalar>& vfs,
	const index m,
	const index n,
	const scalar lambda)
	:
	__vfs(vfs),
	__m(m),
	__n(n),
	__lambda(lambda)
{  
}

template<class kernel>
bool NonLinearSquareProblem<kernel>::isValidInput( const Vector& x ) const
{
	return ( x.size() == __n);
}

template<class kernel>
bool NonLinearSquareProblem<kernel>::isValid() const
{
	return ( __m <= __n);
}

template<class kernel>
const typename kernel::index& NonLinearSquareProblem<kernel>::functiondimension() const
{
	return __m;
}

template<class kernel>
const typename kernel::index& NonLinearSquareProblem<kernel>::variabledimension() const
{
	return __n;
}

template<class kernel>
const typename kernel::scalar& NonLinearSquareProblem<kernel>::lambda() const
{
	return __lambda;
}

template<class kernel>
typename kernel::Vector NonLinearSquareProblem<kernel>::Fvalue(const Vector& x ) const
{
	return __vfs.FunctionValue(x);
}

template<class kernel>
typename kernel::Matrix NonLinearSquareProblem<kernel>::Jacobi(const Vector& x ) const
{
	return __vfs.JacobiFun(x);
}

template<class kernel>
typename kernel::scalar NonLinearSquareProblem<kernel>::objective( const Vector& x ) const
{
	return __vfs.FunctionValue(x).dot(__vfs.FunctionValue(x));
}


/***********************Implementation of VectorProblem************************/


} // End of namespace COPT

#endif
