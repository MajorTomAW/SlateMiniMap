// Author: Tom Werner (dc: majort), 2025 December


#include "Widgets/MiniMap.h"

#include "Slate/SMiniMap.h"

UMiniMap::UMiniMap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
	SetClipping(EWidgetClipping::ClipToBounds);
	UWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMiniMap::SetViewDistance(float InViewDistance)
{
	ViewDistance = InViewDistance;
	if (MyMiniMap.IsValid())
	{
		MyMiniMap->SetViewDistance(ViewDistance);
	}
}

void UMiniMap::SetZoomLevel(float InZoomLevel)
{
	ZoomLevel = InZoomLevel;
	if (MyMiniMap.IsValid())
	{
		MyMiniMap->SetZoomLevel(ZoomLevel);
	}
}

void UMiniMap::SetMiniMapRadius(float InMiniMapRadius)
{
	MiniMapRadius = InMiniMapRadius;
	if (MyMiniMap.IsValid())
	{
		MyMiniMap->SetMiniMapRadius(MiniMapRadius);
	}
}

void UMiniMap::SetMiniMapBrush(FSlateBrush InMiniMapBrush)
{
	MiniMapBrush = InMiniMapBrush;
	if (MyMiniMap.IsValid())
	{
		MyMiniMap->SetMiniMapBrush(&MiniMapBrush);
	}
}

void UMiniMap::SetBackgroundBrush(FSlateBrush InBackgroundBrush)
{
	BackgroundBrush = InBackgroundBrush;
	if (MyMiniMap.IsValid())
	{
		MyMiniMap->SetBackgroundBrush(&BackgroundBrush);
	}
}

void UMiniMap::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyMiniMap.Reset();
}

TSharedRef<SWidget> UMiniMap::RebuildWidget()
{
	// This is a runtime-only widget
	if (!IsDesignTime())
	{
		ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
		if (ensureMsgf(LocalPlayer, TEXT("Attempting to rebuild a UMinimapCanvas without a valid LocalPlayer!")))
		{
			MyMiniMap = SNew(SMiniMap, FLocalPlayerContext(LocalPlayer))
				.MiniMapBrush(&MiniMapBrush)
				.AdditionalMapBrushes(&AdditionalMapBrushes)
				.BackgroundBrush(&BackgroundBrush)
				.MiniMapWorldRadius(MiniMapRadius)
				.ZoomLevel(ZoomLevel)
				.ViewDistance(ViewDistance);
			return MyMiniMap.ToSharedRef();
		}
	}

	// If we're in the editor, we don't want to create the minimap canvas
	// Give it a trivial preview of what the minimap might look like in-game
	if (AdditionalMapBrushes.IsEmpty())
	{
		return SNew(SImage).Image(&MiniMapBrush);
	}
	else
	{
		TSharedRef<SOverlay> Overlay = SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage).Image(&MiniMapBrush)
			];

		for (const FSlateBrush& Brush : AdditionalMapBrushes)
		{
			Overlay->AddSlot()
			[
				SNew(SImage).Image(&Brush)
			];
		}

		return Overlay;
	}
}

#if WITH_EDITOR
const FText UMiniMap::GetPaletteCategory()
{
	return NSLOCTEXT("SlateMiniMap", "MiniMap", "Mini Map");
}
#endif