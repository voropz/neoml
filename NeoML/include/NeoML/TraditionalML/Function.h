/* Copyright © 2017-2023 ABBYY

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
--------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <NeoML/NeoMLDefs.h>
#include <NeoML/TraditionalML/FloatVector.h>
#include <NeoML/TraditionalML/Problem.h>

namespace NeoML {

class IThreadPool;

// An arbitrary function interface
// Used for optimization problems and error functions
class CFunction {
public:
	virtual ~CFunction() = default;

	// The problem dimensions
	virtual int NumberOfDimensions() const = 0;

	// Sets the argument for the subsequent calculations
	// Used for intermediate calculations for the function value, gradient
	// or multiplying hessian by a vector
	virtual void SetArgument( const CFloatVector& x ) = 0;

	// The function value
	virtual double Value() const = 0;

	// The Evaluate function that is a wrapper over SetArgument and Value
	double Evaluate( const CFloatVector& x );
};

// The interface for a function that supports gradient calculation
class CFunctionWithGradient : public CFunction {
public:
	// Function gradient
	virtual CFloatVector Gradient() const = 0;
};

// The interface for a function that supports gradient and hessian calculation
class CFunctionWithHessian : public CFunctionWithGradient {
public:
	// The product of the function hessian by a given vector
	virtual CFloatVector HessianProduct( const CFloatVector& s ) = 0;
};

//------------------------------------------------------------------------------------------------------------
// Main loss functions

// For support-vector machine with a squared hinge loss function:
class NEOML_API CSquaredHinge : public CFunctionWithHessian {
public:
	CSquaredHinge( const IProblem& data, double errorWeight, float l1Coeff, int threadCount );
	~CSquaredHinge() override;

	// The CFunctionWithHessian class methods:
	int NumberOfDimensions() const override { return matrix.Width + 1; }
	void SetArgument( const CFloatVector& w ) override;
	double Value() const override { return value;}
	CFloatVector Gradient() const override { return gradient; }
	CFloatVector HessianProduct( const CFloatVector& s ) override;

protected:
	const CFloatMatrixDesc matrix;
	const float errorWeight;
	const float l1Coeff;
	IThreadPool* const threadPool;

	double value;
	CFloatVector gradient;
	CArray<double> hessian;
	CFloatVector answers;
	CFloatVector weights;
};

//------------------------------------------------------------------------------------------------------------

// Loss function for a regression problem
class NEOML_API CL2Regression : public CFunctionWithHessian {
public:
	CL2Regression( const IRegressionProblem& data, double errorWeight, double p, float l1Coeff, int threadCount );
	~CL2Regression() override;

	// The CFunctionWithHessian class methods:
	int NumberOfDimensions() const override { return matrix.Width + 1; }
	void SetArgument(const CFloatVector& w) override;
	double Value() const override { return value; }
	CFloatVector Gradient() const override { return gradient; }
	CFloatVector HessianProduct(const CFloatVector& s) override;

protected:
	const CFloatMatrixDesc matrix;
	const float errorWeight;
	const float p;
	const float l1Coeff;
	IThreadPool* const threadPool;

	double value;
	CFloatVector gradient;
	CArray<double> hessian;
	CFloatVector answers;
	CFloatVector weights;
};

//------------------------------------------------------------------------------------------------------------

// Logistic regression function
class NEOML_API CLogRegression: public CFunctionWithHessian {
public:
	CLogRegression( const IProblem& _data, double errorWeight, float l1Coeff, int threadCount );
	~CLogRegression() override;

	// The CFunctionWithHessian class methods:
	int NumberOfDimensions() const override { return matrix.Width + 1; }
	void SetArgument( const CFloatVector& w ) override;
	double Value() const override { return value; }
	CFloatVector Gradient() const override { return gradient; }
	CFloatVector HessianProduct( const CFloatVector& s ) override;

protected:
	const CFloatMatrixDesc matrix;
	const float errorWeight;
	const float l1Coeff;
	IThreadPool* const threadPool;

	double value;
	CFloatVector gradient;
	CArray<double> hessian;
	CFloatVector answers;
	CFloatVector weights;
};

//------------------------------------------------------------------------------------------------------------

// Smoothed hinge function
class NEOML_API CSmoothedHinge : public CFunctionWithHessian {
public:
	CSmoothedHinge( const IProblem& data, double errorWeight, float l1Coeff, int threadCount );
	~CSmoothedHinge() override;

	// The CFunctionWithHessian class methods:
	int NumberOfDimensions() const override { return matrix.Width + 1; }
	void SetArgument( const CFloatVector& w ) override;
	double Value() const override { return value; }
	CFloatVector Gradient() const override { return gradient; }
	CFloatVector HessianProduct( const CFloatVector& s ) override;

protected:
	const CFloatMatrixDesc matrix;
	const float errorWeight;
	const float l1Coeff;
	IThreadPool* const threadPool;

	double value;
	CFloatVector gradient;
	CArray<double> hessian;
	CFloatVector answers;
	CFloatVector weights;
};

//------------------------------------------------------------------------------------------------------------

// inline implementation
inline double CFunction::Evaluate( const CFloatVector& x )
{
	SetArgument( x );
	return Value();
}

} // namespace NeoML
