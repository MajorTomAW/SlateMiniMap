// Author: Tom Werner (dc: majort), 2025 December

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MiniMap.generated.h"


class SMiniMap;

UCLASS(meta=(DisplayName="MiniMap"))
class SLATEMINIMAP_API UMiniMap : public UWidget
{
	GENERATED_BODY()

public:
	UMiniMap(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
	/** Brush to render the actual mini map.*/
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush MiniMapBrush;

	/** Additional brushes to render on top of the mini map.*/
	UPROPERTY(EditAnywhere, Category=Appearance)
	TArray<FSlateBrush> AdditionalMapBrushes;
	
	/** Brush to render the map background. */
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush BackgroundBrush;

	/** Extent of the minimap in world space
	 * This is important to be accurate, otherwise your mini map will be off. */
	UPROPERTY(EditAnywhere, Category=Appearance, Getter, Setter, BlueprintSetter="SetMiniMapRadius", BlueprintGetter="GetMiniMapRadius")
	float MiniMapRadius = 100000.f;

	/** View distance used to calculate the size of the minimap rect to display. */
	UPROPERTY(EditAnywhere, Category=Appearance, Getter, Setter, BlueprintSetter="SetViewDistance", BlueprintGetter="GetViewDistance")
	float ViewDistance = 1000.f;
	
	/** Zoom level scalar applied to the view distance. */
	UPROPERTY(EditAnywhere, Category=Appearance, Getter, Setter, BlueprintSetter="SetZoomLevel", BlueprintGetter="GetZoomLevel")
	float ZoomLevel = 1.f;

public:
	/** Sets the ViewDistance of the mini map. */
	UFUNCTION(BlueprintCallable, Category=Widget)
	virtual void SetViewDistance(float InViewDistance);
	/** Returns the view distance. */
	UFUNCTION(BlueprintCallable, Category=Widget)
	float GetViewDistance() const { return ViewDistance; }
	
	/** Sets the ZoomLevel of the mini map. */
	UFUNCTION(BlueprintCallable, Category=Widget)
	virtual void SetZoomLevel(float InZoomLevel);
	/** Returns the zoom level. */
	UFUNCTION(BlueprintCallable, Category=Widget)
	float GetZoomLevel() const { return ZoomLevel; }

	/** Directly sets the mini map radius. */
	UFUNCTION(BlueprintCallable, Category=Widget)
	virtual void SetMiniMapRadius(float InMiniMapRadius);
	/** Returns mini map radius.*/
	UFUNCTION(BlueprintCallable, Category=Widget)
	float GetMiniMapRadius() const { return MiniMapRadius; }

	/** Directly sets the mini map brush. */
	UFUNCTION(BlueprintCallable, Category=Widget)
	virtual void SetMiniMapBrush(FSlateBrush InMiniMapBrush);

	/** Directly sets the mini map background brush. */
	UFUNCTION(BlueprintCallable, Category=Widget)
	virtual void SetBackgroundBrush(FSlateBrush InBackgroundBrush);

protected:
	//~ Begin UWidget Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	//~ End UWidget Interface

protected:
	TSharedPtr<SMiniMap> MyMiniMap;
};
