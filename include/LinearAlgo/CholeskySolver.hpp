//		Copyright (C) Ruimin Wang, ruimin.wang13@gmail.com
//		Copyright (C) MathU

#ifndef CHOLESKY_SOLVER_HPP__
#define CHOLESKY_SOLVER_HPP__

namespace COPT
{

/*			Cholesky factorization for symmetric semidefine matrix.
 *
 *
 */
template<class Matrix>
class Cholesky
	:
	public LinearSolver<Matrix>
{
private:
	typedef typename Matrix::index 				index;
	typedef typename Matrix::scalar 			scalar;
	typedef VectorBase<scalar,index>			Vector;

	/** the data */
	scalar 							*__a;

	/** info */
	index 							__info;

	/** since the input matrix has to be symmetric semidefinite */
	bool validation( const Matrix& mat ) const;

	/** implementation of virtual functions */
	//%{
	void doCompute( const Matrix& mat );
	Vector doSolve( const Vector& b );
	Matrix doSolve( const Matrix& b );
	//%}

public:

	/** constructor and deconstructor */
	//%{
	/** default constructor */
	Cholesky();
	Cholesky(const Matrix& mat);
	~Cholesky();
	//%}

	/** inver matrix */
	Matrix inverse ();

	/** clear */
	void clear();
};

template<class Matrix>
Cholesky<Matrix>::Cholesky()
	:
	__a(NULL)
{
}

template<class Matrix>
Cholesky<Matrix>::Cholesky( const Matrix& mat )
	:
	__a(NULL)
{
	this->compute(mat);
}

template<class Matrix>
Cholesky<Matrix>::~Cholesky()
{
	clear();
}

template<class Matrix>
void Cholesky<Matrix>::doCompute( const Matrix& mat )
{
	if (!validation(mat))
	{
		std::cerr<<"Warning in cholesky solver: the input matrix is not symmetric. Please confirm that!"<<std::endl;
		return;
	}
	clear();
	__a = new scalar[mat.size()];
	this->setRowNum(mat.rows());
	this->setColNum(mat.cols());
	this->setLDA(mat.rows());
	blas::copt_blas_copy(mat.size(),mat.dataPtr(),1,__a,1);
	copt_lapack_potrf('U',this->rowNum(),__a,this->lda(),&__info);
	if( __info != 0 )
	{
		std::cout<<__info<<std::endl;
		std::cerr<<"Warning in Cholesky solver: something computation is wrong!"<<std::endl;
	}
}

template<class Matrix>
typename Cholesky<Matrix>::Vector Cholesky<Matrix>::doSolve( const Vector& b )
{
	if ( this->rowNum() != b.size() )
	{
		std::cerr<<"The order of matrix is "<<this->rowNum()<<" and the dimension of vector is "<<b.size()<<std::endl;
		throw COException("Cholesky solving error: the size if not consistent!");
	}
	Vector result(b);
	copt_lapack_potrs('U',this->rowNum(),1,__a,this->lda(),result.dataPtr(),result.size(),&__info);
	if ( __info != 0 )
		std::cerr<<"Warning in Cholesky solver: solving is wrong!"<<std::endl;
	return result;
}

template<class Matrix>
Matrix Cholesky<Matrix>::doSolve( const Matrix& b )
{
	if ( this->rowNum() != b.rows() )
	{
		std::cerr<<"The order of matrix is "<<this->rowNum()<<" and the dimension of vector is "<<b.rows()<<std::endl;
		throw COException("Cholesky solving error: the size if not consistent!");
	}
	Matrix result(b);
	copt_lapack_potrs('U',this->rowNum(),b.cols(),__a,this->lda(),result.dataPtr(),result.rows(),&__info);
	if ( __info != 0 )
		std::cerr<<"Warning in Cholesky solver: solving is wrong!"<<std::endl;
	return result;
}

template<class Matrix>
bool Cholesky<Matrix>::validation( const Matrix& mat )const
{
	return mat.isSymmetric();
}

template<class Matrix>
Matrix Cholesky<Matrix>::inverse( )
{
	this->squareValidation();
	Matrix result(this->rowNum(),this->rowNum(),__a);
	copt_lapack_potri('U',this->rowNum(),result.dataPtr(),this->lda(),&__info);
	result.setSymmetricFlag(true);
	return result;
}

template<class Matrix>
void Cholesky<Matrix>::clear()
{
	SAFE_DELETE_ARRAY(__a);
}

}

#endif