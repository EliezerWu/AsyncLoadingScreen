/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "STipWidget.h"
#include "LoadingScreenSettings.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "AsyncLoadingScreenLibrary.h"

namespace
{
	TSharedRef<STextBlock> MakeTipLine(const FText& Text, const FTextAppearance& Appearance)
	{
		return SNew(STextBlock)
			.ColorAndOpacity(Appearance.ColorAndOpacity)
			.Font(Appearance.Font)
			.ShadowOffset(Appearance.ShadowOffset)
			.ShadowColorAndOpacity(Appearance.ShadowColorAndOpacity)
			.Justification(Appearance.Justification)
			.Text(Text);
	}
}

int32 STipWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// Update the tip text if Interval > 0 and TipText has more than one element
	if (Interval > 0.0f && TipText.Num() > 1 && TipTextBlock.IsValid())
	{
		TotalDeltaTime += Args.GetDeltaTime();

		if (TotalDeltaTime >= Interval)
		{
			int32 TipIndex = FMath::RandRange(0, TipText.Num() - 1);

			TipTextBlock->SetText(TipText[TipIndex]);

			TotalDeltaTime = 0.0f;
		}
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void STipWidget::Construct(const FArguments& InArgs, const FTipSettings& Settings)
{
	TipText = Settings.TipText;
	Interval = Settings.UpdateInterval;

	if (TipText.Num() == 0)
	{
		return;
	}

	int32 TipIndex = FMath::RandRange(0, TipText.Num() - 1);

	if (Settings.bSetDisplayTipTextManually == true)
	{
		if (TipText.IsValidIndex(UAsyncLoadingScreenLibrary::GetDisplayTipTextIndex()))
		{
			TipIndex = UAsyncLoadingScreenLibrary::GetDisplayTipTextIndex();
			// A manually chosen tip stays on screen; disable the random interval refresh
			Interval = 0.0f;
		}
	}

	TSharedRef<SVerticalBox> Column = SNew(SVerticalBox);

	// Eyebrow: small, wide-tracked label above the headline ("TIP").
	if (!Settings.EyebrowText.IsEmpty())
	{
		Column->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.Padding(FMargin(0.0f, 0.0f, 0.0f, FMath::Max(0.0f, Settings.EyebrowSpacing)))
			[
				MakeTipLine(Settings.EyebrowText, Settings.EyebrowAppearance)
			];
	}

	// Headline: the only line that rotates.
	Column->AddSlot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		[
			SAssignNew(TipTextBlock, STextBlock)
				.ColorAndOpacity(Settings.Appearance.ColorAndOpacity)
				.Font(Settings.Appearance.Font)
				.ShadowOffset(Settings.Appearance.ShadowOffset)
				.ShadowColorAndOpacity(Settings.Appearance.ShadowColorAndOpacity)
				.Justification(Settings.Appearance.Justification)
				.WrapTextAt(Settings.TipWrapAt)
				.Text(TipText[TipIndex])
		];

	// Detail: quiet second line ("JIANBING FACT · 001 / ...").
	if (!Settings.DetailText.IsEmpty())
	{
		Column->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.Padding(FMargin(0.0f, FMath::Max(0.0f, Settings.DetailSpacing), 0.0f, 0.0f))
			[
				MakeTipLine(Settings.DetailText, Settings.DetailAppearance)
			];
	}

	ChildSlot
	[
		Column
	];
}
