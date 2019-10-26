#pragma once

#include"CSVReader.h"

class MusicInfoRead
{
public:
	MusicInfoRead() {};
	~MusicInfoRead() {};

	CSVReader csvReader;
	int infoStartLine = 3;

	//Music information encoding - data structures

	short int melodiesPerFile = 5;
	std::string songName = "";
	int tonic = 0;
	short order[24] = { 0 };

	// Melody Matrix
	int mf_Matrix[5][8] = { 0 };
	int mv_Matrix[5][8] = { 0 };
	int mo_Matrix[5][8] = { 0 };
	int mScales[5] = { 0 };

	int mf_Array[5][64] = { 0 };
	int mv_Array[5][64] = { 0 };
	int mo_Array[5][64] = { 0 };

	// Chord Matrix
	int cf_Matrix[5][8] = { 0 };
	int cv_Matrix[5][8] = { 0 };
	int ct_Matrix[5][8] = { 0 };

	short int cf_Array[5][64] = { 0 };
	short int ct_Array[5][64] = { 0 };
	short int cv_Array[5][64] = { 0 };

	//// Riff Matrix
	//int rf_Matrix[5][8] = { 0 };
	//int rv_Matrix[5][8] = { 0 };
	//int ro_Matrix[5][8] = { 0 };

	//short int rf_Array[5][64] = { 0 };
	//short int rv_Array[5][64] = { 0 };
	//short int ro_Array[5][64] = { 0 };

	//Music information encoding - functions

	void readCustomMusicCSV(std::string melCSVPath)
	{
		bool mf = false; bool cf = false;
		bool mv = false; bool cv = false;
		bool mo = false; bool ct = false;

		int currentPassage = 0;

		int tempArray[8] = { 0 };
		int scaleVar = 0;
		int totalLines = infoStartLine + 30; // 9 Types of info
		for (int i = 0; i < totalLines; i++)
		{
			if (i < infoStartLine)
			{
				csvReader.readMelCSV_Metadata(melCSVPath, &songName, &tonic, order, i);
			}

			else
			{
				csvReader.readMelCSV_Custom( melCSVPath, tempArray, &scaleVar, i, infoStartLine);

				mf = (i < (melodiesPerFile + infoStartLine));
				mv = (i >= (1 * melodiesPerFile + infoStartLine) && i < (2 * melodiesPerFile + infoStartLine));
				mo = (i >= (2 * melodiesPerFile + infoStartLine) && i < (3 * melodiesPerFile + infoStartLine));
				cf = (i >= (3 * melodiesPerFile + infoStartLine) && i < (4 * melodiesPerFile + infoStartLine));
				cv = (i >= (4 * melodiesPerFile + infoStartLine) && i < (5 * melodiesPerFile + infoStartLine));
				ct = (i >= (5 * melodiesPerFile + infoStartLine) && i < (6 * melodiesPerFile + infoStartLine));

				currentPassage = i - infoStartLine;

				for (int j = 0; j < 8; j++)
				{
					if (mf) // Melody Frequency
					{
						mf_Matrix[currentPassage%melodiesPerFile][j] = tempArray[j];
						mScales[currentPassage] = scaleVar;
					}
					if (mv) // Melody Velocity and Note Length
					{
						mv_Matrix[currentPassage%melodiesPerFile][j] = tempArray[j];
					}
					if (mo) // Melody Note Octave
					{
						mo_Matrix[currentPassage%melodiesPerFile][j] = tempArray[j];
					}
					if (cf) // Chord Degree
					{
						cf_Matrix[currentPassage%melodiesPerFile][j] = tempArray[j];
					}
					if (cv) // Chord Velocity
					{
						cv_Matrix[currentPassage%melodiesPerFile][j] = tempArray[j];
					}
					if (ct) // Chord Type
					{
						ct_Matrix[currentPassage%melodiesPerFile][j] = tempArray[j];
					}
				}
			}
		}
	};

	void convertCodeMatricesToArrays()
	{
		int code_mf = 0; int digit_mf = 0;
		int code_mv = 0; int digit_mv = 0;
		int code_mo = 0; int digit_mo = 0;

		int code_cf = 0; int digit_cf = 0;
		int code_cv = 0; int digit_cv = 0;
		int code_ct = 0; int digit_ct = 0;

		/*int code_rf = 0; int digit_rf = 0;
		int code_rv = 0; int digit_rv = 0;
		int code_ro = 0; int digit_ro = 0;*/

		int divisor = 0;

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				code_mf = mf_Matrix[i][j];
				code_mv = mv_Matrix[i][j];
				code_mo = mo_Matrix[i][j];
				
				code_cf = cf_Matrix[i][j];
				code_cv = cv_Matrix[i][j];
				code_ct = ct_Matrix[i][j];
				
				/*code_rf = rf_Matrix[i][j];
				code_rv = rv_Matrix[i][j];
				code_ro = ro_Matrix[i][j];*/

				for (int k = 0; k < 8; k++)
				{
					divisor = pow(10, 8 - k - 1);
					digit_mf = (code_mf / divisor) % 10;
					digit_mv = (code_mv / divisor) % 10;
					digit_mo = (code_mo / divisor) % 10;

					digit_cf = (code_cf / divisor) % 10;
					digit_ct = (code_ct / divisor) % 10;
					digit_cv = (code_cv / divisor) % 10;

					/*digit_rf = (code_rf / divisor) % 10;
					digit_rv = (code_rv / divisor) % 10;
					digit_ro = (code_ro / divisor) % 10;*/
					
					mf_Array[i][8 * j + k] = digit_mf;
					mv_Array[i][8 * j + k] = digit_mv;
					mo_Array[i][8 * j + k] = digit_mo;
					cf_Array[i][8 * j + k] = digit_cf;
					ct_Array[i][8 * j + k] = digit_ct;
					cv_Array[i][8 * j + k] = digit_cv;
					/*rf_Array[i][8 * j + k] = digit_rf;
					rv_Array[i][8 * j + k] = digit_rv;
					ro_Array[i][8 * j + k] = digit_ro;*/
				}
			}
		}
	};
	
};

