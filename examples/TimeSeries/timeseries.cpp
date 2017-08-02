#include "drawproc.h"

#include <cfloat>
#include <stdio.h>

#include "milk-tea-coffee.h"

int rowCount = sizeof(data) / sizeof(data[0]);

static const int COLUMN_YEAR = 0;
static const int COLUMN_MILK = 1;
static const int COLUMN_TEA = 2;
static const int COLUMN_COFFEE = 3;

bool isValid(const int row, const int col);
void drawDataPoints(const int col);
void drawTitle();
void drawYearLabels();


// Data ranges
float dataMin;
float dataMax;

// year ranges
int yearMin;
int yearMax;
int yearInterval = 10;

// volume information
int volumeInterval = 10;
int volumeIntervalMinor = 5;


// plot area
int plotX1, plotY1;
int plotX2, plotY2;
int labelX, labelY;

int currentColumn = COLUMN_MILK;
int columnCount = 3;

const uint8_t * plotFont;


bool isValid(const int row, const int col)
{
	return true;
}

float getColumnMax(int col)
{
	float m = FLT_MIN;
	double value;

	for (int row = 0; row < rowCount; row++) {
		// only consider valid data items
		if (isValid(row, col)) {
			switch (col) {
			case COLUMN_YEAR:
				value = data[row].year;
				break;
			case COLUMN_MILK:
				value = data[row].milk;
				break;
			case COLUMN_TEA:
				value = data[row].tea;
				break;
			case COLUMN_COFFEE:
				value = data[row].coffee;
				break;
			}
			if (value > m) {
				m = value;
			}
		}
	}
	return m;
}

float getDataMax()
{
	float m = FLT_MIN;
	m = MAX(m, getColumnMax(COLUMN_MILK));
	m = MAX(m, getColumnMax(COLUMN_TEA));
	m = MAX(m, getColumnMax(COLUMN_COFFEE));

	return m;
}

float getFloat(const int row, const int col)
{
	float value;

	switch (col) {
	case COLUMN_YEAR:
		value = data[row].year;
		break;
	case COLUMN_MILK:
		value = data[row].milk;
		break;
	case COLUMN_TEA:
		value = data[row].tea;
		break;
	case COLUMN_COFFEE:
		value = data[row].coffee;
		break;
	}
	return value;
}

const char * getColumnName(const int col)
{
	switch (col) {
	case COLUMN_MILK:
		return "Milk";
	case COLUMN_TEA:
		return "Tea";
	case COLUMN_COFFEE:
		return "Coffee";
	}
	return "UNKNOWN";
}

void setup()
{
	size(720, 405);

	dataMin = 0;
	dataMax = ceil(getDataMax()/volumeInterval) * volumeInterval;
	columnCount = 3;

	yearMin = data[0].year;
	yearMax = data[rowCount - 1].year;

	plotX1 = 120;
	plotX2 = width - 80;
	labelX = 50;
	plotY1 = 60;
	plotY2 = height - 70;
	labelY = height - 25;

	//setFont(verdana18_bold);

	//smooth();
}


void drawTitle()
{
	setFont(verdana18_bold);

	// draw title of current plot
	fill(0);
	textSize(20);
	textAlign(TX_LEFT, TX_BOTTOM);
	const char * title = getColumnName(currentColumn);
	text(title, plotX1, plotY1 - 20);
}

void drawDataPoints(const int col)
{
	for (int row = 0; row < rowCount; row++){
		if (isValid(row, col)) {
			float value = getFloat(row, col);
			double x = MAP(data[row].year, yearMin, yearMax, plotX1, plotX2);
			double y = MAP(value, dataMin, dataMax, plotY2, plotY1);
			point(x, y);
		}
	}
}

void drawAxisLabels()
{
	fill(0);
	textSize(13);
	//textLeading(15);

	int yLabel = (plotY1 + plotY2) / 2;
	textAlign(TX_CENTER, TX_CENTER);
	text("Gallons", labelX, yLabel );
	text("consumed", labelX, yLabel+12);
	text("per capita", labelX, yLabel+24);

	textAlign(TX_CENTER, TX_BOTTOM);
	text("Year", (plotX1 + plotX2) / 2, labelY);
}

void drawYearLabels()
{
	char yearstr[10];

	setFont(gse7x11);

	stroke(224, 224, 224, 255);
	strokeWeight(1);

	fill(pBlack);
	//textSize(10);
	textAlign(TX_CENTER, TX_TOP);

	for (int row = 0; row < rowCount; row++) {
		if (data[row].year % yearInterval == 0) {
			float x = MAP(data[row].year, yearMin, yearMax, plotX1, plotX2);
			sprintf_s(yearstr, "%4d", data[row].year);
			text(yearstr, x, plotY2 + 10);
			line(x, plotY1, x, plotY2);
		}
	}
}

void drawVolumeLabels()
{
	char numbuff[256];

	fill(0);
	textSize(10);
	textAlign(TX_RIGHT, TX_CENTER);

	stroke((uint8_t)128);
	strokeWeight(1);

	for (float v = dataMin; v <= dataMax; v += volumeIntervalMinor) {		
		if (int(v) % volumeIntervalMinor == 0) {
			float y = MAP(v, dataMin, dataMax, plotY2, plotY1);
			if (int(v) % volumeInterval == 0)
			{
				if (v == dataMin) {
					textAlign(TX_RIGHT);
				}
				else if (v == dataMax) {
					textAlign(TX_RIGHT, TX_TOP);
				}
				else {
					textAlign(TX_RIGHT, TX_CENTER);
				}

				sprintf_s(numbuff, "%d", (int)floor(v));
				text(numbuff, plotX1 - 10, y);
				line(plotX1 - 4, y, plotX1, y);		// draw a major tick
			}

			line(plotX1 - 2, y, plotX1, y);			// draw a minor tick
		}
	}
}

void draw()
{
	background(224,224,224,255);
	//background(pLightGray);
	//background(aliceblue);

	// plot area as white box
	fill(255);
	rectMode(CORNERS);
	noStroke();
	rect(plotX1, plotY1, plotX2, plotY2);

	drawTitle();

	// plot the actual columnar data
	stroke(0x56, 0x79, 0xc1, 255);
	strokeWeight(5);
	drawDataPoints(currentColumn);

	drawYearLabels();
	drawVolumeLabels();
	drawAxisLabels();
}





void  keyReleased()
{
	switch (keyCode)
	{
	case KC_RIGHT:
		currentColumn++;
		if (currentColumn > COLUMN_COFFEE) {
			currentColumn = COLUMN_MILK;
		}
		break;

	case KC_LEFT:
		currentColumn--;
		if (currentColumn < COLUMN_MILK) {
			currentColumn = COLUMN_COFFEE;
		}
		break;


	case KC_SPACE:
		write_PPM_binary("test_text.ppm", gpb);
		break;

	}

}

