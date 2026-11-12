/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

struct FTipSettings;
class STextBlock;

/**
 * Tip widget
 *
 * Fork change: three stacked lines (eyebrow / headline / detail) instead of a
 * single text block, so the loading tip can match the mockup's 15/28/17px
 * typography. Only the headline rotates.
 */
class STipWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STipWidget) {}

	SLATE_END_ARGS()

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void Construct(const FArguments& InArgs, const FTipSettings& Settings);	

protected:
	// Current total delta time
	mutable float TotalDeltaTime = 0.0f;

	// Interval time (in seconds) to update the tip text, a value less than or equal to 0 will not update the tip text.
	float Interval = 0.0f;

private:
	// Only the headline rotates; eyebrow and detail stay put.
	TSharedPtr<STextBlock> TipTextBlock;

	TArray<FText> TipText;
};
