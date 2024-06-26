/* Copyright © 2017-2020 ABBYY Production LLC

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
#include <NeoML/Dnn/Dnn.h>

namespace NeoML {

// CEltwiseBaseLayer is the base class for layers performing elementwise operations
class NEOML_API CEltwiseBaseLayer : public CBaseLayer {
public:
	void Serialize( CArchive& archive ) override;

protected:
	CEltwiseBaseLayer( IMathEngine& mathEngine, const char* name ) : CBaseLayer( mathEngine, name, false ) {}
	void Reshape() override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseSumLayer implements a layer that adds its inputs element by element
class NEOML_API CEltwiseSumLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseSumLayer )
public:
	explicit CEltwiseSumLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine, "CCnnEltwiseSumLayer" ) {}

	void Serialize( CArchive& archive ) override;

protected:
	void RunOnce() override;
	void BackwardOnce() override;
	int BlobsForBackward() const override { return 0; }
};

NEOML_API CLayerWrapper<CEltwiseSumLayer> Sum();

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseSubLayer implements a layer that subtracts its second inputs from the first element by element
class NEOML_API CEltwiseSubLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseSubLayer )
public:
	explicit CEltwiseSubLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine, "CCnnEltwiseSubLayer" ) {}

	void Serialize( CArchive& archive ) override;

protected:
	void Reshape() override;
	void RunOnce() override;
	void BackwardOnce() override;
	int BlobsForBackward() const override { return 0; }
};

NEOML_API CLayerWrapper<CEltwiseSubLayer> Sub();

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseMulLayer implements a layer that multiplies its inputs element by element
class NEOML_API CEltwiseMulLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseMulLayer )
public:
	explicit CEltwiseMulLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine, "CCnnEltwiseMulLayer" ) {}

	void Serialize( CArchive& archive ) override;

protected:
	void RunOnce() override;
	void BackwardOnce() override;
	int BlobsForBackward() const override { return TInputBlobs; }
};

NEOML_API CLayerWrapper<CEltwiseMulLayer> Mul();

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseNegMulLayer implements a layer that calculates 
// the product of `1 - firstInput[i]` and input[i] for all other inputs
class NEOML_API CEltwiseNegMulLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseNegMulLayer )
public:
	explicit CEltwiseNegMulLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine, "CCnnEltwiseNegMulLayer" ) {}

	void Serialize( CArchive& archive ) override;

protected:
	void Reshape() override;
	void RunOnce() override;
	void BackwardOnce() override;
	int BlobsForBackward() const override { return TInputBlobs; }

private:
	CPtr<CDnnBlob> oneVector;
	CPtr<CDnnBlob> negInputBlob;
};

NEOML_API CLayerWrapper<CEltwiseNegMulLayer> NegMul();

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseMaxLayer implements a layer that finds the maximum among the elements that are at the same position in all input blobs
class NEOML_API CEltwiseMaxLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseMaxLayer )
public:
	explicit CEltwiseMaxLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine, "CCnnEltwiseMaxLayer" ) {}

	void Serialize( CArchive& archive ) override;

protected:
	CArray< CArray<CConstFloatHandle> > vectorsArray;
	CArray< CArray<CFloatHandle> > diffVectorsArray;

	void Reshape() override;
	void RunOnce() override;
	void BackwardOnce() override;
	int BlobsForBackward() const override { return 0; }

private:
	CPtr<CDnnBlob> maxIndices; // the indices of the inputs that had the largest elements
};

NEOML_API CLayerWrapper<CEltwiseMaxLayer> Max();

///////////////////////////////////////////////////////////////////////////////////////////////////////

// CEltwiseDivLayer implements a layer that divides first input by the second input element by element
class NEOML_API CEltwiseDivLayer : public CEltwiseBaseLayer {
	NEOML_DNN_LAYER( CEltwiseDivLayer )
public:
	explicit CEltwiseDivLayer( IMathEngine& mathEngine ) : CEltwiseBaseLayer( mathEngine, "CEltwiseDivLayer" ) {}

	void Serialize( CArchive& archive ) override;

protected:
	void Reshape() override;
	void RunOnce() override;
	void BackwardOnce() override;
	int BlobsForBackward() const override { return TInputBlobs | TOutputBlobs; }
};

NEOML_API CLayerWrapper<CEltwiseDivLayer> Div();

} // namespace NeoML
