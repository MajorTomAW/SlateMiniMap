// Author: Tom Werner (dc: majort), 2026 January


#include "MiniMapProjection.h"

void MiniMap::FMiniMapProjectionData::UpdateProjectionData(
	const FVector& InOrigin,
	const FVector2D& MiniMapExtent,
	const FGeometry& AllottedGeometry)
{
	//UE_LOG(LogTemp, Warning, TEXT("UpdateProjectionData: %s"))
	
	const FTransform2d T = FTransform2d(1.0, -FVector2D(InOrigin));
	const FTransform2d V = FTransform2d(
		(ScreenRect.GetExtent() / MiniMapExtent).GetMin(),
		FVector2D(ScreenRect.GetSize().X * 0.5f, ScreenRect.GetSize().Y * 0.5f));

	Origin = InOrigin;
	WorldToScreenTransform = T.Concatenate(V);
	ScreenToWorldTransform = WorldToScreenTransform.Inverse();
	ScreenRect = FBox2D(FVector2D(0.f), AllottedGeometry.GetLocalSize());
}

FVector2D MiniMap::FMiniMapProjectionData::WorldToScreen(
	const FVector2D& PointInWorldSpace) const
{
	return WorldToScreenTransform.TransformPoint(PointInWorldSpace);
}

FVector2D MiniMap::FMiniMapProjectionData::WorldToScreen(const FVector& PointInWorldSpace) const
{
	return WorldToScreenTransform.TransformPoint(FVector2D(PointInWorldSpace));
}

FBox2D MiniMap::FMiniMapProjectionData::MakeScreenBounds(const FBox2D& WorldBounds) const
{
	return FBox2D(
		WorldToScreen(WorldBounds.Min),
		WorldToScreen(WorldBounds.Max));
}

bool MiniMap::FMiniMapProjectionData::IsWithinScreenRect(const FVector2D& PointInScreenSpace) const
{
	return ScreenRect.IsInside(PointInScreenSpace);
}

bool MiniMap::FMiniMapProjectionData::IsWithinScreenRect(const FBox2D& ScreenBounds) const
{
	return ScreenRect.Overlap(ScreenBounds).bIsValid;
}

FPaintGeometry MiniMap::FMiniMapProjectionData::MakePaintGeometry_FromWP(
	const FBox2D& WorldBounds,
	const FGeometry& AllottedGeometry) const
{
	return MakePaintGeometry_FromSP(MakeScreenBounds(WorldBounds), AllottedGeometry);
}

FString MiniMap::FMiniMapProjectionData::ToDebugString() const
{
	return FString::Printf(TEXT("WorldToScreen: %s\tScreenToWorld: %s"), *WorldToScreenTransform.GetTranslation().ToString(), *ScreenToWorldTransform.GetTranslation().ToString());
}

FPaintGeometry MiniMap::FMiniMapProjectionData::MakePaintGeometry_FromSP(
	const FBox2D& ScreenBounds,
	const FGeometry& AllottedGeometry) const
{
	return AllottedGeometry.ToPaintGeometry(
			ScreenBounds.GetSize(),
			FSlateLayoutTransform(ScreenBounds.Min));
}
