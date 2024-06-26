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

#include <common.h>
#pragma hdrstop

#include <NeoML/Dnn/Dnn.h>
#include <NeoML/Dnn/Layers/SplitLayer.h>

namespace NeoML {

CBaseSplitLayer::CBaseSplitLayer( IMathEngine& mathEngine, TBlobDim _dimension, const char* name ) :
	CBaseLayer( mathEngine, name, false ),
	dimension( _dimension )
{
}

void CBaseSplitLayer::SetOutputCounts(const CArray<int>& _outputCounts)
{
	_outputCounts.CopyTo(outputCounts);
	ForceReshape();
}

void CBaseSplitLayer::SetOutputCounts2(int count0)
{
	outputCounts.SetSize(1);
	outputCounts[0] = count0;
	ForceReshape();
}

void CBaseSplitLayer::SetOutputCounts3(int count0, int count1)
{
	outputCounts.SetSize(2);
	outputCounts[0] = count0;
	outputCounts[1] = count1;
	ForceReshape();
}

void CBaseSplitLayer::SetOutputCounts4(int count0, int count1, int count2)
{
	outputCounts.SetSize(3);
	outputCounts[0] = count0;
	outputCounts[1] = count1;
	outputCounts[2] = count2;
	ForceReshape();
}

static const int BaseSplitLayerVersion = 2000;

void CBaseSplitLayer::Serialize( CArchive& archive )
{
	archive.SerializeVersion( BaseSplitLayerVersion, CDnn::ArchiveMinSupportedVersion );
	CBaseLayer::Serialize( archive );

	archive.Serialize(outputCounts);
}

void CBaseSplitLayer::Reshape()
{
	CheckInputs();

	CBlobDesc pattern = inputDescs[0];
	int restDimSize = pattern.DimSize(dimension);

	for( int i = 0; i < outputCounts.Size(); ++i ) {
		pattern.SetDimSize(dimension, outputCounts[i]);
		outputDescs[i] = pattern;
		restDimSize -= outputCounts[i];
	}

	NeoAssert(restDimSize >= 0);
	if(restDimSize > 0) {
		NeoAssert( GetOutputCount() == outputCounts.Size() + 1 );
		pattern.SetDimSize(dimension, restDimSize);
		outputDescs[outputCounts.Size()] = pattern;
	}
}

void CBaseSplitLayer::RunOnce()
{
	CDnnBlob::SplitByDim( MathEngine(), dimension, inputBlobs[0], outputBlobs );
}

void CBaseSplitLayer::BackwardOnce()
{
	CDnnBlob::MergeByDim( MathEngine(), dimension, outputDiffBlobs, inputDiffBlobs[0] );
}

//----------------------------------------------------------------------------------------------------

static const int SplitChannelsLayerVersion = 2000;

void CSplitChannelsLayer::Serialize( CArchive& archive )
{
	archive.SerializeVersion( SplitChannelsLayerVersion, CDnn::ArchiveMinSupportedVersion );
	CBaseSplitLayer::Serialize( archive );
}

CLayerWrapper<CSplitChannelsLayer> SplitChannels( const CArray<int>& outputCounts )
{
	return CLayerWrapper<CSplitChannelsLayer>( "SplitChannels", [&outputCounts]( CSplitChannelsLayer* result ) {
		result->SetOutputCounts( outputCounts );
	} );
}

CLayerWrapper<CSplitChannelsLayer> SplitChannels( int output0, int output1, int output2 )
{
	return CLayerWrapper<CSplitChannelsLayer>( "SplitChannels", [=]( CSplitChannelsLayer* result ) {
		if( output1 == 0 ) {
			result->SetOutputCounts2( output0 );
		} else if( output2 == 0 ) {
			result->SetOutputCounts3( output0, output1 );
		} else {
			result->SetOutputCounts4( output0, output1, output2 );
		}
	} );
}

//----------------------------------------------------------------------------------------------------

static const int SplitDepthLayerVersion = 2000;

void CSplitDepthLayer::Serialize( CArchive& archive )
{
	archive.SerializeVersion( SplitDepthLayerVersion, CDnn::ArchiveMinSupportedVersion );
	CBaseSplitLayer::Serialize( archive );
}

CLayerWrapper<CSplitDepthLayer> SplitDepth( const CArray<int>& outputCounts )
{
	return CLayerWrapper<CSplitDepthLayer>( "SplitDepth", [&outputCounts]( CSplitDepthLayer* result ) {
		result->SetOutputCounts( outputCounts );
	} );
}

CLayerWrapper<CSplitDepthLayer> SplitDepth( int output0, int output1, int output2 )
{
	return CLayerWrapper<CSplitDepthLayer>( "SplitDepth", [=]( CSplitDepthLayer* result ) {
		if( output1 == 0 ) {
			result->SetOutputCounts2( output0 );
		} else if( output2 == 0 ) {
			result->SetOutputCounts3( output0, output1 );
		} else {
			result->SetOutputCounts4( output0, output1, output2 );
		}
	} );
}

//----------------------------------------------------------------------------------------------------

static const int SplitWidthLayerVersion = 2000;

void CSplitWidthLayer::Serialize( CArchive& archive )
{
	archive.SerializeVersion( SplitWidthLayerVersion, CDnn::ArchiveMinSupportedVersion );
	CBaseSplitLayer::Serialize( archive );
}

CLayerWrapper<CSplitWidthLayer> SplitWidth( const CArray<int>& outputCounts )
{
	return CLayerWrapper<CSplitWidthLayer>( "SplitWidth", [&outputCounts]( CSplitWidthLayer* result ) {
		result->SetOutputCounts( outputCounts );
	} );
}

CLayerWrapper<CSplitWidthLayer> SplitWidth( int output0, int output1, int output2 )
{
	return CLayerWrapper<CSplitWidthLayer>( "SplitWidth", [=]( CSplitWidthLayer* result ) {
		if( output1 == 0 ) {
			result->SetOutputCounts2( output0 );
		} else if( output2 == 0 ) {
			result->SetOutputCounts3( output0, output1 );
		} else {
			result->SetOutputCounts4( output0, output1, output2 );
		}
	} );
}

//----------------------------------------------------------------------------------------------------

static const int SplitHeightLayerVersion = 2000;

void CSplitHeightLayer::Serialize( CArchive& archive )
{
	archive.SerializeVersion( SplitHeightLayerVersion, CDnn::ArchiveMinSupportedVersion );
	CBaseSplitLayer::Serialize( archive );
}

CLayerWrapper<CSplitHeightLayer> SplitHeight( const CArray<int>& outputCounts )
{
	return CLayerWrapper<CSplitHeightLayer>( "SplitHeight", [&outputCounts]( CSplitHeightLayer* result ) {
		result->SetOutputCounts( outputCounts );
	} );
}

CLayerWrapper<CSplitHeightLayer> SplitHeight( int output0, int output1, int output2 )
{
	return CLayerWrapper<CSplitHeightLayer>( "SplitHeight", [=]( CSplitHeightLayer* result ) {
		if( output1 == 0 ) {
			result->SetOutputCounts2( output0 );
		} else if( output2 == 0 ) {
			result->SetOutputCounts3( output0, output1 );
		} else {
			result->SetOutputCounts4( output0, output1, output2 );
		}
	} );
}

//----------------------------------------------------------------------------------------------------

static const int SplitListSizeLayerVersion = 0;

void CSplitListSizeLayer::Serialize( CArchive& archive )
{
	archive.SerializeVersion( SplitListSizeLayerVersion );
	CBaseSplitLayer::Serialize( archive );
}

CLayerWrapper<CSplitListSizeLayer> SplitListSize( const CArray<int>& outputCounts )
{
	return CLayerWrapper<CSplitListSizeLayer>( "SplitListSize", [&outputCounts]( CSplitListSizeLayer* result ) {
		result->SetOutputCounts( outputCounts );
		} );
}

CLayerWrapper<CSplitListSizeLayer> SplitListSize( int output0, int output1, int output2 )
{
	return CLayerWrapper<CSplitListSizeLayer>( "SplitListSize", [=]( CSplitListSizeLayer* result ) {
		if( output1 == 0 ) {
			result->SetOutputCounts2( output0 );
		} else if( output2 == 0 ) {
			result->SetOutputCounts3( output0, output1 );
		} else {
			result->SetOutputCounts4( output0, output1, output2 );
		}
		} );
}

//----------------------------------------------------------------------------------------------------

static const int SplitBatchWidthLayerVersion = 2000;

void CSplitBatchWidthLayer::Serialize( CArchive& archive )
{
	archive.SerializeVersion( SplitBatchWidthLayerVersion, CDnn::ArchiveMinSupportedVersion );
	CBaseSplitLayer::Serialize( archive );
}

CLayerWrapper<CSplitBatchWidthLayer> SplitBatchWidth( const CArray<int>& outputCounts )
{
	return CLayerWrapper<CSplitBatchWidthLayer>( "SplitBatchWidth", [&outputCounts]( CSplitBatchWidthLayer* result ) {
		result->SetOutputCounts( outputCounts );
	} );
}

CLayerWrapper<CSplitBatchWidthLayer> SplitBatchWidth( int output0, int output1, int output2 )
{
	return CLayerWrapper<CSplitBatchWidthLayer>( "SplitBatchWidth", [=]( CSplitBatchWidthLayer* result ) {
		if( output1 == 0 ) {
			result->SetOutputCounts2( output0 );
		} else if( output2 == 0 ) {
			result->SetOutputCounts3( output0, output1 );
		} else {
			result->SetOutputCounts4( output0, output1, output2 );
		}
	} );
}

//----------------------------------------------------------------------------------------------------

static const int SplitBatchLengthLayerVersion = 0;

void CSplitBatchLengthLayer::Serialize( CArchive& archive )
{
	archive.SerializeVersion( SplitBatchLengthLayerVersion );
	CBaseSplitLayer::Serialize( archive );
}

CLayerWrapper<CSplitBatchLengthLayer> SplitBatchLength( const CArray<int>& outputCounts )
{
	return CLayerWrapper<CSplitBatchLengthLayer>( "SplitBatchLength", [&outputCounts]( CSplitBatchLengthLayer* result ) {
		result->SetOutputCounts( outputCounts );
		} );
}

CLayerWrapper<CSplitBatchLengthLayer> SplitBatchLength( int output0, int output1, int output2 )
{
	return CLayerWrapper<CSplitBatchLengthLayer>( "SplitBatchLength", [=]( CSplitBatchLengthLayer* result ) {
		if( output1 == 0 ) {
			result->SetOutputCounts2( output0 );
		} else if( output2 == 0 ) {
			result->SetOutputCounts3( output0, output1 );
		} else {
			result->SetOutputCounts4( output0, output1, output2 );
		}
		} );
}

} // namespace NeoML
