// Author: Tom Werner (dc: majort), 2025 December


#include "Slate/SMiniMap.h"

#include "SlateOptMacros.h"


SMiniMap::SMiniMap()
{
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMiniMap::Construct(
	const FArguments& InArgs,
	const FLocalPlayerContext& InLocalPlayerContext)
{
	BackgroundBrush = InArgs._BackgroundBrush;
	MiniMapBrush = InArgs._MiniMapBrush;
	AdditionalMapBrushes = InArgs._AdditionalMapBrushes;
	MiniMapWorldRadius = InArgs._MiniMapWorldRadius;
	ZoomLevel = InArgs._ZoomLevel;
	ViewDistance = InArgs._ViewDistance;
	DirectionalLettersFontInfo = InArgs._FontInfo;

	LocalPlayerContext = InLocalPlayerContext;
	
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/

	SetClipping(EWidgetClipping::ClipToBounds);
}

void SMiniMap::SetZoomLevel(TAttribute<float> InZoomLevel)
{
	ZoomLevel = InZoomLevel;
}

void SMiniMap::SetViewDistance(TAttribute<float> InViewDistance)
{
	ViewDistance = InViewDistance;
}

void SMiniMap::SetMiniMapRadius(TAttribute<float> InMiniMapRadius)
{
	MiniMapWorldRadius = InMiniMapRadius;
}

void SMiniMap::SetMiniMapBrush(const FSlateBrush* InMiniMapBrush)
{
	MiniMapBrush = InMiniMapBrush;
}

void SMiniMap::SetBackgroundBrush(const FSlateBrush* InBackgroundBrush)
{
	BackgroundBrush = InBackgroundBrush;
}

bool SMiniMap::GetPlayerLocation(FVector& Location, FRotator& Rotation) const
{
	if (const ULocalPlayer* Player = GetLocalPlayer())
	{
		if (APlayerController* Controller = Player->GetPlayerController(GetWorld()))
		{
			if (APawn* Pawn = Controller->GetPawn())
			{
				Location = Pawn->GetActorLocation();
				Rotation = Pawn->GetActorRotation();
				return true;
			}
		}
	}
	
	return false;
}

FVector2d SMiniMap::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	if (MiniMapBrush)
	{
		return MiniMapBrush->GetImageSize();
	}

	return FVector2d(MiniMapWorldRadius.Get());
}

bool SMiniMap::ComputeVolatility() const
{
	return false;
}

int32 SMiniMap::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_SMiniMap_OnPaint)
	{
		FVector Location;
		FRotator Rotation;
		if (GetPlayerLocation(Location, Rotation))
		{
			const FVector2D PlayerExtent(ViewDistance.Get() * ZoomLevel.Get());
			MiniMapProjectionData.UpdateProjectionData(Location, PlayerExtent, AllottedGeometry);
		}
	}

	LayerId = PaintMiniMap(AllottedGeometry, MyCullingRect, OutDrawElements, ++LayerId);
	return LayerId;
}

int32 SMiniMap::PaintMiniMap(
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId) const
{
	if (BackgroundBrush)
	{
		FSlateDrawElement::MakeBox(
			   OutDrawElements,
			   ++LayerId,
			   AllottedGeometry.ToPaintGeometry(),
			   BackgroundBrush,
			   ESlateDrawEffect::None,
			   BackgroundBrush->TintColor.GetSpecifiedColor());
	}

	const float MiniMapHalfRadius = 0.5f * MiniMapWorldRadius.Get();
	const float MiniMapRadiusRatio = MiniMapHalfRadius / MiniMapBrush->GetImageSize().X;

	// Minimap is background, any other should be on top of it
	if (MiniMapBrush)
	{
		const FBox2D WorldMiniMapBounds = FBox2D(FVector2D(-MiniMapHalfRadius), FVector2D(MiniMapHalfRadius));
		const FPaintGeometry WorldImageGeometry = MiniMapProjectionData.MakePaintGeometry_FromWP(
			WorldMiniMapBounds,
			AllottedGeometry);
		
		FSlateDrawElement::MakeBox(
		  OutDrawElements,
		  ++LayerId,
		  WorldImageGeometry,
		  MiniMapBrush);
	}

	if (AdditionalMapBrushes)
	{
		for (FSlateBrush Brush : *AdditionalMapBrushes)
		{
			const FBox2D WorldMiniMapBounds = FBox2D(
				FVector2D(-MiniMapRadiusRatio * Brush.GetImageSize().X),
				FVector2D(MiniMapRadiusRatio * Brush.GetImageSize().X));
			const FPaintGeometry WorldImageGeometry = MiniMapProjectionData.MakePaintGeometry_FromWP(
				WorldMiniMapBounds,
				AllottedGeometry);
			
			FSlateDrawElement::MakeBox(
			  OutDrawElements,
			  ++LayerId,
			  WorldImageGeometry,
			  &Brush);
		}
	}
	

	return LayerId;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
