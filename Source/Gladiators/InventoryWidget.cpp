// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/Image.h"

void UInventoryWidget::ManageInventory()
{
	if (Slot1 && Slot2 && Slot3)
	{
		if (InventoryCount == 0)
		{
			Slot1->SetVisibility(ESlateVisibility::Hidden);
			Slot2->SetVisibility(ESlateVisibility::Hidden);
			Slot3->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (InventoryCount ==1)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("InventoryCount up"));
			Slot1->SetVisibility(ESlateVisibility::Visible);
			Slot2->SetVisibility(ESlateVisibility::Hidden);
			Slot3->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (InventoryCount == 2)
		{
			Slot1->SetVisibility(ESlateVisibility::Visible);
			Slot2->SetVisibility(ESlateVisibility::Visible);
			Slot3->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (InventoryCount ==3)
		{
			Slot3->SetVisibility(ESlateVisibility::Visible);
			Slot1->SetVisibility(ESlateVisibility::Visible);
			Slot2->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
