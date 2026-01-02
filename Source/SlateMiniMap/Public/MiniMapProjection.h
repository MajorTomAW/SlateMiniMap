// Author: Tom Werner (dc: majort), 2026 January

#pragma once

class UMiniMapIndicator;

namespace MiniMap
{
	struct SLATEMINIMAP_API FMiniMapProjectionData
	{
	public:
		FMiniMapProjectionData()
			: ScreenRect(ForceInit)
			, Origin(ForceInit)
		{
		}

		void UpdateProjectionData(const FVector& InOrigin, const FVector2D& MiniMapExtent, const FGeometry& AllottedGeometry);

		FVector2D WorldToScreen(const FVector2D& PointInWorldSpace) const;
		FVector2D WorldToScreen(const FVector& PointInWorldSpace) const;
		FBox2D MakeScreenBounds(const FBox2D& WorldBounds) const;
		bool IsWithinScreenRect(const FVector2D& PointInScreenSpace) const;
		bool IsWithinScreenRect(const FBox2D& ScreenBounds) const;
		FPaintGeometry MakePaintGeometry_FromSP(const FBox2D& ScreenBounds, const FGeometry& AllottedGeometry) const;
		FPaintGeometry MakePaintGeometry_FromWP(const FBox2D& WorldBounds, const FGeometry& AllottedGeometry) const;

		FBox2D GetScreenRect() const { return ScreenRect;}
		FVector GetOrigin() const {return Origin; }

		FString ToDebugString() const;
		
	private:
		FBox2D ScreenRect;
		FVector Origin;
		FTransform2d WorldToScreenTransform;
		FTransform2d ScreenToWorldTransform;
	};
}