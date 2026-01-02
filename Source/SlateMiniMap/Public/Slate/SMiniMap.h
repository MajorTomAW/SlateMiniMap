// Author: Tom Werner (dc: majort), 2025 December

#pragma once

#include "CoreMinimal.h"
#include "MiniMapProjection.h"
#include "Widgets/SLeafWidget.h"

class SMiniMap : public SLeafWidget
{
	using ThisClass = SMiniMap;
	using Super = SLeafWidget;
public:
	SLATE_BEGIN_ARGS(SMiniMap)
		: _MiniMapBrush(nullptr)
		, _BackgroundBrush(nullptr)
		, _MiniMapWorldRadius(10000.f)
		, _ZoomLevel(1.f)
		{
		}
		SLATE_ARGUMENT(const FSlateBrush*, MiniMapBrush)
		SLATE_ARGUMENT(const FSlateBrush*, BackgroundBrush)
		SLATE_ARGUMENT(FSlateFontInfo, FontInfo)
		SLATE_ATTRIBUTE(float, MiniMapWorldRadius)
		SLATE_ATTRIBUTE(float, ZoomLevel)
	SLATE_END_ARGS()

	SMiniMap();
	
public:
	void Construct(const FArguments& InArgs, const FLocalPlayerContext& InLocalPlayerContext);

	/** Sets the zoom level. */
	void SetZoomLevel(TAttribute<float> InZoomLevel);

	/** Sets the mini map radius. */
	void SetMiniMapRadius(TAttribute<float> InMiniMapRadius);

	/** Sets the mini map brush. */
	void SetMiniMapBrush(const FSlateBrush* InMiniMapBrush);

	/** Sets the background brush. */
	void SetBackgroundBrush(const FSlateBrush* InBackgroundBrush);

	UWorld* GetWorld() { return LocalPlayerContext.GetWorld(); }
	const UWorld* GetWorld() const { return LocalPlayerContext.GetWorld(); }

	ULocalPlayer* GetLocalPlayer() { return LocalPlayerContext.GetLocalPlayer(); }
	const ULocalPlayer* GetLocalPlayer() const { return LocalPlayerContext.GetLocalPlayer(); }

protected:
	bool GetPlayerLocation(FVector& Location, FRotator& Rotation) const;
	
	//~ Begin SWidget Interface
	virtual FVector2d ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual bool ComputeVolatility() const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	//~ End SWidget Interface

	virtual int32 PaintMiniMap(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

private:
	/** Passed-in args. */
	TAttribute<float> MiniMapWorldRadius;
	TAttribute<float> ZoomLevel;
	const FSlateBrush* MiniMapBrush = nullptr;
	const FSlateBrush* BackgroundBrush = nullptr;
	FSlateFontInfo DirectionalLettersFontInfo;

	/** Info about the owning local player. */
	FLocalPlayerContext LocalPlayerContext;

	/** Data used for projection. */
	mutable MiniMap::FMiniMapProjectionData MiniMapProjectionData;
};
