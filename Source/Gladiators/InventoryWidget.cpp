// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/Image.h"



void UInventoryWidget::AddToInventory()
{
	InventoryCount++;
}

void UInventoryWidget::RemoveFromInventory()
{
	InventoryCount--;
}



void UInventoryWidget::ManageInventory()
{
	if (InventoryCount == 0)
	{
		EmptyImage1->RemoveFromParent();
	}
	else if (InventoryCount == 1)
	{
		//Show 1 Inventory Slot
	}
	else if (InventoryCount ==2)
	{
		//Show 2 Inventory slots
	}
	else
	{
		//Show 3 inventory slots
	}
}
