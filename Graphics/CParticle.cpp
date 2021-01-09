﻿#include "CParticle.h"
#include <iostream>
#include "SceneItem.h"
#include "MutexFactory.h"
#include <time.h>

Graphics::CParticle::CParticle(string particleName) :
	m_pShader(nullptr)
{
	// only for debug purposes
	m_particleName = particleName;
}

Graphics::CParticle::~CParticle()
{

	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	if (m_pShader != nullptr)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
}

void Graphics::CParticle::Create()
{
	static Float boxVertexes[] =
	{
-1, 0, 1,
1, 0, 1,
1, 0, -1,
-1, 0, 1,
1, 0, -1,
-1, 0, -1,
-1, 0.01, 1,
1, 0.01, 1,
1, 0.01, -1,
-1, 0.01, 1,
1, 0.01, -1,
-1, 0.01, -1,
-1, 0.02, 1,
1, 0.02, 1,
1, 0.02, -1,
-1, 0.02, 1,
1, 0.02, -1,
-1, 0.02, -1,
-1, 0.03, 1,
1, 0.03, 1,
1, 0.03, -1,
-1, 0.03, 1,
1, 0.03, -1,
-1, 0.03, -1,
-1, 0.04, 1,
1, 0.04, 1,
1, 0.04, -1,
-1, 0.04, 1,
1, 0.04, -1,
-1, 0.04, -1,
-1, 0.05, 1,
1, 0.05, 1,
1, 0.05, -1,
-1, 0.05, 1,
1, 0.05, -1,
-1, 0.05, -1,
-1, 0.06, 1,
1, 0.06, 1,
1, 0.06, -1,
-1, 0.06, 1,
1, 0.06, -1,
-1, 0.06, -1,
-1, 0.07, 1,
1, 0.07, 1,
1, 0.07, -1,
-1, 0.07, 1,
1, 0.07, -1,
-1, 0.07, -1,
-1, 0.08, 1,
1, 0.08, 1,
1, 0.08, -1,
-1, 0.08, 1,
1, 0.08, -1,
-1, 0.08, -1,
-1, 0.09, 1,
1, 0.09, 1,
1, 0.09, -1,
-1, 0.09, 1,
1, 0.09, -1,
-1, 0.09, -1,
-1, 0.1, 1,
1, 0.1, 1,
1, 0.1, -1,
-1, 0.1, 1,
1, 0.1, -1,
-1, 0.1, -1,
-1, 0.11, 1,
1, 0.11, 1,
1, 0.11, -1,
-1, 0.11, 1,
1, 0.11, -1,
-1, 0.11, -1,
-1, 0.12, 1,
1, 0.12, 1,
1, 0.12, -1,
-1, 0.12, 1,
1, 0.12, -1,
-1, 0.12, -1,
-1, 0.13, 1,
1, 0.13, 1,
1, 0.13, -1,
-1, 0.13, 1,
1, 0.13, -1,
-1, 0.13, -1,
-1, 0.14, 1,
1, 0.14, 1,
1, 0.14, -1,
-1, 0.14, 1,
1, 0.14, -1,
-1, 0.14, -1,
-1, 0.15, 1,
1, 0.15, 1,
1, 0.15, -1,
-1, 0.15, 1,
1, 0.15, -1,
-1, 0.15, -1,
-1, 0.16, 1,
1, 0.16, 1,
1, 0.16, -1,
-1, 0.16, 1,
1, 0.16, -1,
-1, 0.16, -1,
-1, 0.17, 1,
1, 0.17, 1,
1, 0.17, -1,
-1, 0.17, 1,
1, 0.17, -1,
-1, 0.17, -1,
-1, 0.18, 1,
1, 0.18, 1,
1, 0.18, -1,
-1, 0.18, 1,
1, 0.18, -1,
-1, 0.18, -1,
-1, 0.19, 1,
1, 0.19, 1,
1, 0.19, -1,
-1, 0.19, 1,
1, 0.19, -1,
-1, 0.19, -1,
-1, 0.2, 1,
1, 0.2, 1,
1, 0.2, -1,
-1, 0.2, 1,
1, 0.2, -1,
-1, 0.2, -1,
-1, 0.21, 1,
1, 0.21, 1,
1, 0.21, -1,
-1, 0.21, 1,
1, 0.21, -1,
-1, 0.21, -1,
-1, 0.22, 1,
1, 0.22, 1,
1, 0.22, -1,
-1, 0.22, 1,
1, 0.22, -1,
-1, 0.22, -1,
-1, 0.23, 1,
1, 0.23, 1,
1, 0.23, -1,
-1, 0.23, 1,
1, 0.23, -1,
-1, 0.23, -1,
-1, 0.24, 1,
1, 0.24, 1,
1, 0.24, -1,
-1, 0.24, 1,
1, 0.24, -1,
-1, 0.24, -1,
-1, 0.25, 1,
1, 0.25, 1,
1, 0.25, -1,
-1, 0.25, 1,
1, 0.25, -1,
-1, 0.25, -1,
-1, 0.26, 1,
1, 0.26, 1,
1, 0.26, -1,
-1, 0.26, 1,
1, 0.26, -1,
-1, 0.26, -1,
-1, 0.27, 1,
1, 0.27, 1,
1, 0.27, -1,
-1, 0.27, 1,
1, 0.27, -1,
-1, 0.27, -1,
-1, 0.28, 1,
1, 0.28, 1,
1, 0.28, -1,
-1, 0.28, 1,
1, 0.28, -1,
-1, 0.28, -1,
-1, 0.29, 1,
1, 0.29, 1,
1, 0.29, -1,
-1, 0.29, 1,
1, 0.29, -1,
-1, 0.29, -1,
-1, 0.3, 1,
1, 0.3, 1,
1, 0.3, -1,
-1, 0.3, 1,
1, 0.3, -1,
-1, 0.3, -1,
-1, 0.31, 1,
1, 0.31, 1,
1, 0.31, -1,
-1, 0.31, 1,
1, 0.31, -1,
-1, 0.31, -1,
-1, 0.32, 1,
1, 0.32, 1,
1, 0.32, -1,
-1, 0.32, 1,
1, 0.32, -1,
-1, 0.32, -1,
-1, 0.33, 1,
1, 0.33, 1,
1, 0.33, -1,
-1, 0.33, 1,
1, 0.33, -1,
-1, 0.33, -1,
-1, 0.34, 1,
1, 0.34, 1,
1, 0.34, -1,
-1, 0.34, 1,
1, 0.34, -1,
-1, 0.34, -1,
-1, 0.35, 1,
1, 0.35, 1,
1, 0.35, -1,
-1, 0.35, 1,
1, 0.35, -1,
-1, 0.35, -1,
-1, 0.36, 1,
1, 0.36, 1,
1, 0.36, -1,
-1, 0.36, 1,
1, 0.36, -1,
-1, 0.36, -1,
-1, 0.37, 1,
1, 0.37, 1,
1, 0.37, -1,
-1, 0.37, 1,
1, 0.37, -1,
-1, 0.37, -1,
-1, 0.38, 1,
1, 0.38, 1,
1, 0.38, -1,
-1, 0.38, 1,
1, 0.38, -1,
-1, 0.38, -1,
-1, 0.39, 1,
1, 0.39, 1,
1, 0.39, -1,
-1, 0.39, 1,
1, 0.39, -1,
-1, 0.39, -1,
-1, 0.4, 1,
1, 0.4, 1,
1, 0.4, -1,
-1, 0.4, 1,
1, 0.4, -1,
-1, 0.4, -1,
-1, 0.41, 1,
1, 0.41, 1,
1, 0.41, -1,
-1, 0.41, 1,
1, 0.41, -1,
-1, 0.41, -1,
-1, 0.42, 1,
1, 0.42, 1,
1, 0.42, -1,
-1, 0.42, 1,
1, 0.42, -1,
-1, 0.42, -1,
-1, 0.43, 1,
1, 0.43, 1,
1, 0.43, -1,
-1, 0.43, 1,
1, 0.43, -1,
-1, 0.43, -1,
-1, 0.44, 1,
1, 0.44, 1,
1, 0.44, -1,
-1, 0.44, 1,
1, 0.44, -1,
-1, 0.44, -1,
-1, 0.45, 1,
1, 0.45, 1,
1, 0.45, -1,
-1, 0.45, 1,
1, 0.45, -1,
-1, 0.45, -1,
-1, 0.46, 1,
1, 0.46, 1,
1, 0.46, -1,
-1, 0.46, 1,
1, 0.46, -1,
-1, 0.46, -1,
-1, 0.47, 1,
1, 0.47, 1,
1, 0.47, -1,
-1, 0.47, 1,
1, 0.47, -1,
-1, 0.47, -1,
-1, 0.48, 1,
1, 0.48, 1,
1, 0.48, -1,
-1, 0.48, 1,
1, 0.48, -1,
-1, 0.48, -1,
-1, 0.49, 1,
1, 0.49, 1,
1, 0.49, -1,
-1, 0.49, 1,
1, 0.49, -1,
-1, 0.49, -1,
-1, 0.5, 1,
1, 0.5, 1,
1, 0.5, -1,
-1, 0.5, 1,
1, 0.5, -1,
-1, 0.5, -1,
-1, 0.51, 1,
1, 0.51, 1,
1, 0.51, -1,
-1, 0.51, 1,
1, 0.51, -1,
-1, 0.51, -1,
-1, 0.52, 1,
1, 0.52, 1,
1, 0.52, -1,
-1, 0.52, 1,
1, 0.52, -1,
-1, 0.52, -1,
-1, 0.53, 1,
1, 0.53, 1,
1, 0.53, -1,
-1, 0.53, 1,
1, 0.53, -1,
-1, 0.53, -1,
-1, 0.54, 1,
1, 0.54, 1,
1, 0.54, -1,
-1, 0.54, 1,
1, 0.54, -1,
-1, 0.54, -1,
-1, 0.55, 1,
1, 0.55, 1,
1, 0.55, -1,
-1, 0.55, 1,
1, 0.55, -1,
-1, 0.55, -1,
-1, 0.56, 1,
1, 0.56, 1,
1, 0.56, -1,
-1, 0.56, 1,
1, 0.56, -1,
-1, 0.56, -1,
-1, 0.57, 1,
1, 0.57, 1,
1, 0.57, -1,
-1, 0.57, 1,
1, 0.57, -1,
-1, 0.57, -1,
-1, 0.58, 1,
1, 0.58, 1,
1, 0.58, -1,
-1, 0.58, 1,
1, 0.58, -1,
-1, 0.58, -1,
-1, 0.59, 1,
1, 0.59, 1,
1, 0.59, -1,
-1, 0.59, 1,
1, 0.59, -1,
-1, 0.59, -1,
-1, 0.6, 1,
1, 0.6, 1,
1, 0.6, -1,
-1, 0.6, 1,
1, 0.6, -1,
-1, 0.6, -1,
-1, 0.61, 1,
1, 0.61, 1,
1, 0.61, -1,
-1, 0.61, 1,
1, 0.61, -1,
-1, 0.61, -1,
-1, 0.62, 1,
1, 0.62, 1,
1, 0.62, -1,
-1, 0.62, 1,
1, 0.62, -1,
-1, 0.62, -1,
-1, 0.63, 1,
1, 0.63, 1,
1, 0.63, -1,
-1, 0.63, 1,
1, 0.63, -1,
-1, 0.63, -1,
-1, 0.64, 1,
1, 0.64, 1,
1, 0.64, -1,
-1, 0.64, 1,
1, 0.64, -1,
-1, 0.64, -1,
-1, 0.65, 1,
1, 0.65, 1,
1, 0.65, -1,
-1, 0.65, 1,
1, 0.65, -1,
-1, 0.65, -1,
-1, 0.66, 1,
1, 0.66, 1,
1, 0.66, -1,
-1, 0.66, 1,
1, 0.66, -1,
-1, 0.66, -1,
-1, 0.67, 1,
1, 0.67, 1,
1, 0.67, -1,
-1, 0.67, 1,
1, 0.67, -1,
-1, 0.67, -1,
-1, 0.68, 1,
1, 0.68, 1,
1, 0.68, -1,
-1, 0.68, 1,
1, 0.68, -1,
-1, 0.68, -1,
-1, 0.69, 1,
1, 0.69, 1,
1, 0.69, -1,
-1, 0.69, 1,
1, 0.69, -1,
-1, 0.69, -1,
-1, 0.7, 1,
1, 0.7, 1,
1, 0.7, -1,
-1, 0.7, 1,
1, 0.7, -1,
-1, 0.7, -1,
-1, 0.71, 1,
1, 0.71, 1,
1, 0.71, -1,
-1, 0.71, 1,
1, 0.71, -1,
-1, 0.71, -1,
-1, 0.72, 1,
1, 0.72, 1,
1, 0.72, -1,
-1, 0.72, 1,
1, 0.72, -1,
-1, 0.72, -1,
-1, 0.73, 1,
1, 0.73, 1,
1, 0.73, -1,
-1, 0.73, 1,
1, 0.73, -1,
-1, 0.73, -1,
-1, 0.74, 1,
1, 0.74, 1,
1, 0.74, -1,
-1, 0.74, 1,
1, 0.74, -1,
-1, 0.74, -1,
-1, 0.75, 1,
1, 0.75, 1,
1, 0.75, -1,
-1, 0.75, 1,
1, 0.75, -1,
-1, 0.75, -1,
-1, 0.76, 1,
1, 0.76, 1,
1, 0.76, -1,
-1, 0.76, 1,
1, 0.76, -1,
-1, 0.76, -1,
-1, 0.77, 1,
1, 0.77, 1,
1, 0.77, -1,
-1, 0.77, 1,
1, 0.77, -1,
-1, 0.77, -1,
-1, 0.78, 1,
1, 0.78, 1,
1, 0.78, -1,
-1, 0.78, 1,
1, 0.78, -1,
-1, 0.78, -1,
-1, 0.79, 1,
1, 0.79, 1,
1, 0.79, -1,
-1, 0.79, 1,
1, 0.79, -1,
-1, 0.79, -1,
-1, 0.8, 1,
1, 0.8, 1,
1, 0.8, -1,
-1, 0.8, 1,
1, 0.8, -1,
-1, 0.8, -1,
-1, 0.81, 1,
1, 0.81, 1,
1, 0.81, -1,
-1, 0.81, 1,
1, 0.81, -1,
-1, 0.81, -1,
-1, 0.82, 1,
1, 0.82, 1,
1, 0.82, -1,
-1, 0.82, 1,
1, 0.82, -1,
-1, 0.82, -1,
-1, 0.83, 1,
1, 0.83, 1,
1, 0.83, -1,
-1, 0.83, 1,
1, 0.83, -1,
-1, 0.83, -1,
-1, 0.84, 1,
1, 0.84, 1,
1, 0.84, -1,
-1, 0.84, 1,
1, 0.84, -1,
-1, 0.84, -1,
-1, 0.85, 1,
1, 0.85, 1,
1, 0.85, -1,
-1, 0.85, 1,
1, 0.85, -1,
-1, 0.85, -1,
-1, 0.86, 1,
1, 0.86, 1,
1, 0.86, -1,
-1, 0.86, 1,
1, 0.86, -1,
-1, 0.86, -1,
-1, 0.87, 1,
1, 0.87, 1,
1, 0.87, -1,
-1, 0.87, 1,
1, 0.87, -1,
-1, 0.87, -1,
-1, 0.88, 1,
1, 0.88, 1,
1, 0.88, -1,
-1, 0.88, 1,
1, 0.88, -1,
-1, 0.88, -1,
-1, 0.89, 1,
1, 0.89, 1,
1, 0.89, -1,
-1, 0.89, 1,
1, 0.89, -1,
-1, 0.89, -1,
-1, 0.9, 1,
1, 0.9, 1,
1, 0.9, -1,
-1, 0.9, 1,
1, 0.9, -1,
-1, 0.9, -1,
-1, 0.91, 1,
1, 0.91, 1,
1, 0.91, -1,
-1, 0.91, 1,
1, 0.91, -1,
-1, 0.91, -1,
-1, 0.92, 1,
1, 0.92, 1,
1, 0.92, -1,
-1, 0.92, 1,
1, 0.92, -1,
-1, 0.92, -1,
-1, 0.93, 1,
1, 0.93, 1,
1, 0.93, -1,
-1, 0.93, 1,
1, 0.93, -1,
-1, 0.93, -1,
-1, 0.94, 1,
1, 0.94, 1,
1, 0.94, -1,
-1, 0.94, 1,
1, 0.94, -1,
-1, 0.94, -1,
-1, 0.95, 1,
1, 0.95, 1,
1, 0.95, -1,
-1, 0.95, 1,
1, 0.95, -1,
-1, 0.95, -1,
-1, 0.96, 1,
1, 0.96, 1,
1, 0.96, -1,
-1, 0.96, 1,
1, 0.96, -1,
-1, 0.96, -1,
-1, 0.97, 1,
1, 0.97, 1,
1, 0.97, -1,
-1, 0.97, 1,
1, 0.97, -1,
-1, 0.97, -1,
-1, 0.98, 1,
1, 0.98, 1,
1, 0.98, -1,
-1, 0.98, 1,
1, 0.98, -1,
-1, 0.98, -1,
-1, 0.99, 1,
1, 0.99, 1,
1, 0.99, -1,
-1, 0.99, 1,
1, 0.99, -1,
-1, 0.99, -1
	};


	//Float boxTextCoords[] =
	//{
	//	0, 0,
	//	0, 1,
	//	1, 1,
	//	1, 0
	//};

	static Float indices[] = 
	{
	1, 2, 3,
4, 5, 6,
7, 8, 9,
10, 11, 12,
13, 14, 15,
16, 17, 18,
19, 20, 21,
22, 23, 24,
25, 26, 27,
28, 29, 30,
31, 32, 33,
34, 35, 36,
37, 38, 39,
40, 41, 42,
43, 44, 45,
46, 47, 48,
49, 50, 51,
52, 53, 54,
55, 56, 57,
58, 59, 60,
61, 62, 63,
64, 65, 66,
67, 68, 69,
70, 71, 72,
73, 74, 75,
76, 77, 78,
79, 80, 81,
82, 83, 84,
85, 86, 87,
88, 89, 90,
91, 92, 93,
94, 95, 96,
97, 98, 99,
100, 101, 102,
103, 104, 105,
106, 107, 108,
109, 110, 111,
112, 113, 114,
115, 116, 117,
118, 119, 120,
121, 122, 123,
124, 125, 126,
127, 128, 129,
130, 131, 132,
133, 134, 135,
136, 137, 138,
139, 140, 141,
142, 143, 144,
145, 146, 147,
148, 149, 150,
151, 152, 153,
154, 155, 156,
157, 158, 159,
160, 161, 162,
163, 164, 165,
166, 167, 168,
169, 170, 171,
172, 173, 174,
175, 176, 177,
178, 179, 180,
181, 182, 183,
184, 185, 186,
187, 188, 189,
190, 191, 192,
193, 194, 195,
196, 197, 198,
199, 200, 201,
202, 203, 204,
205, 206, 207,
208, 209, 210,
211, 212, 213,
214, 215, 216,
217, 218, 219,
220, 221, 222,
223, 224, 225,
226, 227, 228,
229, 230, 231,
232, 233, 234,
235, 236, 237,
238, 239, 240,
241, 242, 243,
244, 245, 246,
247, 248, 249,
250, 251, 252,
253, 254, 255,
256, 257, 258,
259, 260, 261,
262, 263, 264,
265, 266, 267,
268, 269, 270,
271, 272, 273,
274, 275, 276,
277, 278, 279,
280, 281, 282,
283, 284, 285,
286, 287, 288,
289, 290, 291,
292, 293, 294,
295, 296, 297,
298, 299, 300,
301, 302, 303,
304, 305, 306,
307, 308, 309,
310, 311, 312,
313, 314, 315,
316, 317, 318,
319, 320, 321,
322, 323, 324,
325, 326, 327,
328, 329, 330,
331, 332, 333,
334, 335, 336,
337, 338, 339,
340, 341, 342,
343, 344, 345,
346, 347, 348,
349, 350, 351,
352, 353, 354,
355, 356, 357,
358, 359, 360,
361, 362, 363,
364, 365, 366,
367, 368, 369,
370, 371, 372,
373, 374, 375,
376, 377, 378,
379, 380, 381,
382, 383, 384,
385, 386, 387,
388, 389, 390,
391, 392, 393,
394, 395, 396,
397, 398, 399,
400, 401, 402,
403, 404, 405,
406, 407, 408,
409, 410, 411,
412, 413, 414,
415, 416, 417,
418, 419, 420,
421, 422, 423,
424, 425, 426,
427, 428, 429,
430, 431, 432,
433, 434, 435,
436, 437, 438,
439, 440, 441,
442, 443, 444,
445, 446, 447,
448, 449, 450,
451, 452, 453,
454, 455, 456,
457, 458, 459,
460, 461, 462,
463, 464, 465,
466, 467, 468,
469, 470, 471,
472, 473, 474,
475, 476, 477,
478, 479, 480,
481, 482, 483,
484, 485, 486,
487, 488, 489,
490, 491, 492,
493, 494, 495,
496, 497, 498,
499, 500, 501,
502, 503, 504,
505, 506, 507,
508, 509, 510,
511, 512, 513,
514, 515, 516,
517, 518, 519,
520, 521, 522,
523, 524, 525,
526, 527, 528,
529, 530, 531,
532, 533, 534,
535, 536, 537,
538, 539, 540,
541, 542, 543,
544, 545, 546,
547, 548, 549,
550, 551, 552,
553, 554, 555,
556, 557, 558,
559, 560, 561,
562, 563, 564,
565, 566, 567,
568, 569, 570,
571, 572, 573,
574, 575, 576,
577, 578, 579,
580, 581, 582,
583, 584, 585,
586, 587, 588,
589, 590, 591,
592, 593, 594,
595, 596, 597,
598, 599, 600
};

	m_indicesCount = sizeof(indices) / sizeof(indices[0]);

	// first, try to compile the shader
	m_pShader = generateShader("particles");


	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertexes), &boxVertexes[0], GL_STATIC_DRAW);

	// element buffer object allocation
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// check for GL errors
	Int32 error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << std::endl;
	}
}

void Graphics::CParticle::Update(float dt)
{

}

void Graphics::CParticle::Draw(const SceneItem& si, float dt, bool isRenderingShadows)
{
	if (isRenderingShadows)
		return;

	// clear any non-treated Gl errors
	Int32 glErr = glGetError();
	if (glErr != 0)
		DEBUG_OUT("Untreated error has been found");

	si.SetUpScene(m_pShader);

	// draw all meshes mesh
	//glBindVertexArray(VBO);
	//glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, m_indicesCount);

	// check for GL errors
	Int32 error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << std::endl;
	}

	// always good practice to set everything back to defaults once configured.
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
}

#pragma optimize( "", off )
cwc::glShader* Graphics::CParticle::generateShader(const string& shaderName)
{
	// return value
	bool status = true;
	// start loading measuring time
	clock_t start = clock();

	// read data from file
	cwc::glShader* pShader = nullptr;
	char vertexFilename[128], fragmentFilename[128], geometryFilename[128];
	sprintf(vertexFilename, "./Assets/%sVertexshader.txt", shaderName.data());
	sprintf(fragmentFilename, "./Assets/%sFragmentshader.txt", shaderName.data());
	// there should be a specialized class for this whole thing
	cwc::glShaderManager shaderLoader;

	bool isGeometryFileNotFound = false;

	// checks if the geometry shader actually is required
	pShader = shaderLoader.loadfromFile(vertexFilename, fragmentFilename);


	if (pShader == nullptr)
	{
		printf("<!> Failed to parse shader files [%s]\n", shaderName.data());
		status = false;
	}

	// time measurement
	printf(" [*] loading shader [%s] %.2fms\n", shaderName.data(), (float)(clock() - start));

	return pShader;
}
#pragma optimize( "", on )

