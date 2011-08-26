#include "ImageScreen.h"
// Include NativeUI.

#include <mastdlib.h>
#include <mavsprintf.h>
#include "WidgetUtil.h"



/**
 * This method implements a custom event listener.
 * Widget events are sent as custom events.
 */
void ImageScreen::customEvent(const MAEvent& event)
{
	// Check if this is a widget event.
	if (EVENT_TYPE_WIDGET == event.type)
	{
		// Get the widget event data structure.
		MAWidgetEventData* eventData = (MAWidgetEventData*) event.data;

		// Here we handle clicked events.
		if (MAW_EVENT_CLICKED == eventData->eventType)
		{
			if(mOKButton == eventData->widgetHandle)
			{
				isViewed = false;
				maWidgetStackScreenPop(mStackScreen);
			}

		}
	}
}

int ImageScreen::initialize(MAHandle stackScreen)
{
	mStackScreen = stackScreen;
	createUI();
}


void ImageScreen::createUI()
{
	// Create a Native UI screen. As the screen is a member
		// variable (also called instance variable) we have
		// prefixed the variable name with "m".
		mScreen = maWidgetCreate(MAW_SCREEN);

		// Create the screen's main layout widget.
		mMainLayoutWidget = maWidgetCreate(MAW_VERTICAL_LAYOUT);

		// Make the layout fill the entire screen. For properties that
		// take an integer parameter we use the widgetSetPropertyInt
		// function, for properties that takes a string parameter
		// we use the maWidgetSetProperty function.
		widgetSetPropertyInt(
			mMainLayoutWidget,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mMainLayoutWidget,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);

		// the second layout is a horizontal layout that
		//contains the buttons that control the camera

		// Add the layout as the root of the screen's widget tree.
		maWidgetAddChild(mScreen, mMainLayoutWidget);

		mImageWidget = maWidgetCreate(MAW_IMAGE);
		widgetSetPropertyInt(
				mImageWidget,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				mImageWidget,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		maWidgetAddChild(mMainLayoutWidget, mImageWidget);


		mOKButton = maWidgetCreate(MAW_BUTTON);
		widgetSetPropertyInt(
				mOKButton,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				mOKButton,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);
		maWidgetSetProperty(
				mOKButton,
			MAW_LABEL_TEXT,
			"Back to Main Screen");
		maWidgetAddChild(mMainLayoutWidget, mOKButton);
}


void ImageScreen::pushImageScreen()
{
	isViewed = true;
	maWidgetStackScreenPush(mStackScreen, mScreen);
}

void ImageScreen::setImageDataHandle(MAHandle dataHandle)
{
		if(imageHandle != 0) {
			maDestroyObject(imageHandle);
		}
		imageHandle = maCreatePlaceholder();
		int res = maCreateImageFromData(imageHandle, dataHandle, 0, maGetDataSize(dataHandle));
		if(res != RES_OK)
		{
			maPanic(res, "failed to create the image");
		}

		//destroy the cmaera preview and replace it with an image widget
		maWidgetSetProperty(mImageWidget, MAW_IMAGE_SCALE_MODE, "scalePreserveAspect");
		res = widgetSetPropertyInt(
				mImageWidget,
				MAW_IMAGE_IMAGE,
				imageHandle);
}