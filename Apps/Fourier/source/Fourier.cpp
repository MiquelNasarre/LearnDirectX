#include "Fourier.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "IG_Fourier.h"

#define INTERPRET_FIGURE_VIEW(view, call) \
if (view == -1) harmonics.call; \
else if (view > -1) Figure[view]->call; \
else DataPlots[-view - 2]->call

#define INTERPRET_FIGURE_VIEW_NT(view, call) \
if (view == -1) harmonics.call; \
else if (view > -1) Figure[view]->call

int				IG_DATA::L = 7;
int				IG_DATA::M = -4;
float			IG_DATA::theta = 0.f;
float			IG_DATA::phi = 0.f;
bool			IG_DATA::UPDATE = false;
bool			IG_DATA::UPDATE_CURVES = false;
bool			IG_DATA::CURVES = false;
bool			IG_DATA::MENU = true;

bool 			IG_DATA::DOUBLE_VIEW = false;
int 			IG_DATA::FIGURE_VIEW = -1;
int 			IG_DATA::SECOND_VIEW = -1;
Vector2i*		IG_DATA::PAIRS = NULL;
unsigned int	IG_DATA::PAIRS_SIZE = 0u;
bool			IG_DATA::ALREADY_EXISTS = false;
int				IG_DATA::COPY = -1;

bool			IG_DATA::CALCULATE_FIGURE = false;
bool			IG_DATA::FIGURE_FILE = true;
bool			IG_DATA::LOADING = false;
const char*		IG_DATA::FILENAME = (const char*)calloc(100, sizeof(char));
unsigned int	IG_DATA::MAXL = 0u;
unsigned int	IG_DATA::NFIG = 0u;
unsigned int	IG_DATA::NPLOT = 0u;
Vector2i		IG_DATA::WindowDim = { 0, 0 };

unsigned int	IG_DATA::NINT = 0u;
float			IG_DATA::TVALUE = 0.f;
int				IG_DATA::INTERPOLATION_MENU = -1;
int**			IG_DATA::INTERPOLATION_DATA = NULL;
unsigned int*	IG_DATA::INTERPOLATION_DATA_SIZE = NULL;
bool			IG_DATA::ADD_INTERPOLATION = false;
bool			IG_DATA::DELETE_INTERPOLATION = false;
int				IG_DATA::ADD_FIGURE = -1;
int				IG_DATA::DELETE_FIGURE = -1;

int				IG_DATA::INTERPOLATED_VIEW = -1;

bool			IG_DATA::UPDATE_TEXTURE = false;
_float4color	IG_DATA::TEXTURE = { -1.f,0.f,0.f,0.f };

int						IG_DATA::UPDATE_LIGHT = -1;
IG_DATA::lightsource*	IG_DATA::LIGHTS = (IG_DATA::lightsource*)calloc(sizeof(IG_DATA::lightsource), 8);

Fourier::Fourier()
	: window(960, 720, "Fourier", "", true)
{
	window.setFramerateLimit(60);

	IG_DATA::LIGHTS[0].is_on = true;
	IG_DATA::LIGHTS[0].color = { 1.f, 1.f, 1.f, 1.f };
	IG_DATA::LIGHTS[0].intensities = { 600.f, 320.f, 0.f, 0.f };
	IG_DATA::LIGHTS[0].position = { 30.f, 10.f, 20.f, 0.f };

	harmonics.create(window.graphics, &C, 1u);
}

int Fourier::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void Fourier::drag_dynamic_space()
{
	Vector3f obs = window.graphics.getObserver();
	Vector3f ex = -(obs * Vector3f(0.f, 0.f, 1.f)).normalize();
	Vector3f ey = ex * obs;

	Vector3f lastMouse = (-obs + (ex * (lastPos.x - window.getDimensions().x / 2) + ey * (lastPos.y - window.getDimensions().y / 2)) / scale).normalize();
	lastPos = Mouse::getPosition();
	Vector3f newMouse = (-obs + (ex * (lastPos.x - window.getDimensions().x / 2) + ey * (lastPos.y - window.getDimensions().y / 2)) / scale).normalize();

	Vector3f axis0 = lastMouse * newMouse;
	if (!axis0) axis0 = newMouse;
	axis0.normalize();
	float dangle0 = -acosf(lastMouse ^ newMouse);
	if ((lastMouse ^ newMouse) > 1.f)dangle0 = 0.f;
	if (lastMouse == newMouse) dangle0 = 0.f;

	constexpr float s = 1.f / 1.5f;

	Quaternion newRot = rotationQuaternion(newMouse, Mouse::getWheel() / 18000.f) * rotationQuaternion(axis0, dangle0) * rotationQuaternion(axis, (dangle * (1 - s + fabs(axis ^ newMouse) * s)));

	dangle = 2 * acosf(newRot.r);
	axis = newRot.getVector();
	if (!axis)axis = newMouse;
	axis.normalize();
}

void Fourier::drag_dynamic_plane()
{
	Vector3f obs = window.graphics.getObserver();
	Vector3f ex = -(obs * Vector3f(0.f, 0.f, 1.f)).normalize();
	Vector3f ey = ex * obs;

	Vector2i movement = Mouse::getPosition() - lastPos;
	lastPos = Mouse::getPosition();

	Vector3f desiredAxis;
	float desiredAngle;

	if (!movement)
	{
		desiredAxis = obs;
		desiredAngle = Mouse::getWheel() / 18000.f;
		dangle *= 0.90f;
	}
	else
	{
		desiredAxis = (movement.y * ex - movement.x * ey).normalize();
		desiredAngle = 2 * movement.abs() / scale;
	}

	Vector3f preAxis = axis;
	axis = (axis * dangle + desiredAxis * desiredAngle);
	if (axis)axis.normalize();
	else axis = desiredAxis;
	dangle *= axis ^ preAxis;
	dangle += (desiredAxis ^ axis) * (desiredAngle - dangle) / 20.f;

	if (dangle > 0.2f)dangle = 0.2f;
	if (dangle < -0.2f)dangle = -0.2f;
}

void Fourier::magneticReturn()
{
	if (rotation.r < 0)rotation = -rotation;
	constexpr float pull = 0.03f;

	float angle = 2 * acosf(rotation.r);
	Vector3f axisRot = rotation.getVector();

	Quaternion newRot;
	if (angle < pull)
		newRot = rotationQuaternion(axisRot, -pull / 40.f) * rotationQuaternion(axis, dangle);
	else
		newRot = rotationQuaternion(axisRot, -angle / 40.f) * rotationQuaternion(axis, dangle);
	
	axis = newRot.getVector();
	dangle = 2 * acos(newRot.r);

	dangle *= 0.95f;

	if ((fabs(angle) < 0.01f && fabs(dangle) < 0.005f))
	{
		rotation = 1.f;
		axis = { 1.f,0.f,0.f };
		dangle = 0.f;
		returning = false;

	}
}

void Fourier::strictReturn()
{
	dangle = 0.f;

	if (rotation.r < 0)rotation = -rotation;

	float angle = 2 * acosf(rotation.r);
	Vector3f axisRot = rotation.getVector();

	axis = axisRot;
	dangle = -powf(angle, 0.65f) / 40.f;

	if (fabs(angle) < 0.01f)
	{
		rotation = 1.f;
		strict = false;
		dangle = 0.f;
	}
}

void Fourier::eventManager()
{
	//	Enable disable ImGui

	static bool persists = false;
	if (Keyboard::isKeyPressed('M'))
	{
		if (persists == false)
		{
			if (IG_DATA::MENU)
				IG_DATA::MENU = false;
			else
				IG_DATA::MENU = true;
		}
		persists = true;
	}
	else
		persists = false;

	// Full screen

	if (Keyboard::isKeyPressed(VK_F11))
		window.setFullScreen(true);

	if (Keyboard::isKeyPressed(VK_ESCAPE))
		window.setFullScreen(false);

	// returning motion

	if (Keyboard::isKeyPressed('R'))
		magneticReturn();

	if (Keyboard::isKeyPressed('T'))
		strictReturn();

	if (Keyboard::isKeyPressed('S'))
		dangle = 0.f;

	// Quaternion motion

	if (Mouse::isButtonPressed(Mouse::Left) && !dragging)
	{
		dragging = true;
		lastPos = Mouse::getPosition();
	}

	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging)
	{
		if (IG_DATA::DOUBLE_VIEW)
			drag_dynamic_plane();
		else
			drag_dynamic_space();
	}


	else
		scale *= powf(1.1f, Mouse::getWheel() / 120.f);

	rotation *= rotationQuaternion(axis, dangle * window.getFrameTime() * 60.f);

	IG_DATA::WindowDim = window.getDimensions();

	if (!(rotation.abs() < 10.f))
	{
		rotation = 1.f;
		axis = { 1.f,1.f,1.f };
		dangle = 0.f;
	}

	// Multithread

	if (IG_DATA::CALCULATE_FIGURE)
	{
		IG_DATA::CALCULATE_FIGURE = false;
		IG_DATA::LOADING = true;

		FourierSurface** tFigure = (FourierSurface**)calloc(IG_DATA::NFIG + 1, sizeof(void*));
		for (unsigned int i = 0; i < IG_DATA::NFIG; i++)
			tFigure[i] = Figure[i];
		if(IG_DATA::NFIG)
			free(Figure);
		Figure = tFigure;
		Figure[IG_DATA::NFIG] = new(FourierSurface);

		if (IG_DATA::FIGURE_FILE)
		{
			extractedFigure = FourierSurface::FileManager::extractFigureFromFile(IG_DATA::FILENAME);

			if (!IG_DATA::ALREADY_EXISTS)
			{
				Polihedron** tPlots = (Polihedron**)calloc(IG_DATA::NPLOT + 1, sizeof(void*));
				for (unsigned int i = 0; i < IG_DATA::NPLOT; i++)
					tPlots[i] = DataPlots[i];
				if (IG_DATA::NPLOT)
					free(DataPlots);
				DataPlots = tPlots;
				DataPlots[IG_DATA::NPLOT] = new(Polihedron);
				std::thread(createPlotAsync, &window.graphics, DataPlots[IG_DATA::NPLOT], extractedFigure, &Fplot, &mtx).detach();
			}
			else
			{
				Fplot = true;
			}

			std::thread(calculateCoefficientsAsync, &coef, extractedFigure, IG_DATA::MAXL, &Fcoef).detach();
			std::thread(createFigureAsync, &window.graphics, Figure[IG_DATA::NFIG], &coef, (IG_DATA::MAXL + 1) * (IG_DATA::MAXL + 1), &Ffigu, &mtx, &Fcoef).detach();
		}
		else
		{
			coef = FourierSurface::FileManager::loadCoefficientsFromFile(IG_DATA::FILENAME);
			Fcoef = true;
			std::thread(createFigureAsync, &window.graphics, Figure[IG_DATA::NFIG], &coef, FourierSurface::FileManager::ncoefFromFile(IG_DATA::FILENAME), &Ffigu, &mtx, &Fcoef).detach();
		}
	}

	if (Ffigu && !IG_DATA::FIGURE_FILE)
	{
		Ffigu = false;
		IG_DATA::NFIG++;
		IG_DATA::LOADING = false;
		IG_DATA::FIGURE_VIEW = IG_DATA::NFIG - 1;
		IG_DATA::UPDATE_CURVES = true;
		IG_DATA::UPDATE_LIGHT = -2;
		IG_DATA::DOUBLE_VIEW = false;
		free(coef);
	}

	if (Ffigu && Fplot)
	{
		Ffigu = false;
		Fplot = false;


		IG_DATA::NFIG++;
		IG_DATA::DOUBLE_VIEW = true;
		IG_DATA::LOADING = false;
		IG_DATA::FIGURE_VIEW = IG_DATA::NFIG - 1;
		IG_DATA::UPDATE_CURVES = true;
		IG_DATA::UPDATE_LIGHT = -2;

		if (IG_DATA::ALREADY_EXISTS)
		{
			IG_DATA::SECOND_VIEW = IG_DATA::COPY;
			IG_DATA::ALREADY_EXISTS = false;
		}
		else
		{
			IG_DATA::NPLOT++;
			IG_DATA::SECOND_VIEW = -int(IG_DATA::NPLOT) - 1;
		}

		free((void*)extractedFigure[0]);
		free((void*)extractedFigure[1]);
		free((void*)extractedFigure[2]);
		free(extractedFigure);
		free(coef);
	}

	//	Shape updates

	if (IG_DATA::UPDATE)
	{
		IG_DATA::UPDATE = false;

		C.L = (unsigned int)IG_DATA::L;
		C.M = IG_DATA::M;

		harmonics.updateShape(window.graphics, &C, 1u);

		if (IG_DATA::CURVES)
			harmonics.updateCurves(window.graphics, IG_DATA::phi, IG_DATA::theta);
	}

	if (IG_DATA::UPDATE_CURVES)
	{
		IG_DATA::UPDATE_CURVES = false;

		INTERPRET_FIGURE_VIEW_NT(IG_DATA::FIGURE_VIEW, updateCurves(window.graphics, IG_DATA::phi, IG_DATA::theta));

		if (IG_DATA::DOUBLE_VIEW)
			{INTERPRET_FIGURE_VIEW_NT(IG_DATA::SECOND_VIEW, updateCurves(window.graphics, IG_DATA::phi, IG_DATA::theta));}
	}

	//	Set lights & textures

	int l = IG_DATA::UPDATE_LIGHT;
	if (l == -2) {
		for (int i = 0; i < 8; i++) {
			harmonics.updateLight(window.graphics, i, IG_DATA::LIGHTS[i].intensities, IG_DATA::LIGHTS[i].color, IG_DATA::LIGHTS[i].position);
			for (unsigned int j = 0; j < IG_DATA::NFIG; j++)
				Figure[j]->updateLight(window.graphics, i, IG_DATA::LIGHTS[i].intensities, IG_DATA::LIGHTS[i].color, IG_DATA::LIGHTS[i].position);
		}
	}
	if (l > -1) {
		harmonics.updateLight(window.graphics, l, IG_DATA::LIGHTS[l].intensities, IG_DATA::LIGHTS[l].color, IG_DATA::LIGHTS[l].position);
		for (unsigned int j = 0; j < IG_DATA::NFIG; j++)
			Figure[j]->updateLight(window.graphics, l, IG_DATA::LIGHTS[l].intensities, IG_DATA::LIGHTS[l].color, IG_DATA::LIGHTS[l].position);
		IG_DATA::UPDATE_LIGHT = -1;
	}

	if (IG_DATA::UPDATE_TEXTURE)
	{
		IG_DATA::UPDATE_TEXTURE = false;
		if (IG_DATA::TEXTURE.r == -1.f)
		{
			INTERPRET_FIGURE_VIEW_NT(IG_DATA::FIGURE_VIEW, updateTexture(window.graphics, Color::White, true));
			if(IG_DATA::DOUBLE_VIEW)
				{INTERPRET_FIGURE_VIEW_NT(IG_DATA::SECOND_VIEW, updateTexture(window.graphics, Color::White, true));}
		}
		else if (IG_DATA::TEXTURE.g == -1.f)
		{
			INTERPRET_FIGURE_VIEW_NT(IG_DATA::FIGURE_VIEW, updateTexture(window.graphics, Color::White, false, true));
			if(IG_DATA::DOUBLE_VIEW)
				{INTERPRET_FIGURE_VIEW_NT(IG_DATA::SECOND_VIEW, updateTexture(window.graphics, Color::White, false, true));}
		}
		else
		{
			INTERPRET_FIGURE_VIEW_NT(IG_DATA::FIGURE_VIEW, updateTexture(window.graphics, Color((float*)&IG_DATA::TEXTURE)));
			if (IG_DATA::DOUBLE_VIEW)
				{INTERPRET_FIGURE_VIEW_NT(IG_DATA::SECOND_VIEW, updateTexture(window.graphics, Color((float*)&IG_DATA::TEXTURE)));}
		}

	}

	//	Double view

	if (IG_DATA::DOUBLE_VIEW)
	{
		INTERPRET_FIGURE_VIEW(IG_DATA::FIGURE_VIEW, updateScreenPosition(window.graphics, {-0.5f, 0.f }));
		INTERPRET_FIGURE_VIEW(IG_DATA::SECOND_VIEW, updateScreenPosition(window.graphics, { 0.5f, 0.f }));
	}
	else
		{INTERPRET_FIGURE_VIEW(IG_DATA::FIGURE_VIEW, updateScreenPosition(window.graphics, { 0.f, 0.f }));}

	//	Interpolation

	if (IG_DATA::ADD_INTERPOLATION)
	{
		InterpolatedString** temp = (InterpolatedString**)calloc(IG_DATA::NINT + 1, sizeof(void*));
		for (unsigned int i = 0; i < IG_DATA::NINT; i++)
			temp[i] = Interpolations[i];
		if (Interpolations)
			free(Interpolations);
		Interpolations = temp;

		Interpolations[IG_DATA::NINT] = new(InterpolatedString);

		IG_DATA::DOUBLE_VIEW = false;
		IG_DATA::ADD_INTERPOLATION = false;
		IG_DATA::INTERPOLATED_VIEW = IG_DATA::NINT;
		IG_DATA::NINT++;

	}

	if (IG_DATA::DELETE_INTERPOLATION)
	{
		Interpolations[IG_DATA::INTERPOLATION_MENU]->~InterpolatedString();

		for (unsigned int i = IG_DATA::INTERPOLATION_MENU; i < IG_DATA::NINT - 1; i++)
			Interpolations[i] = Interpolations[i + 1];

		IG_DATA::INTERPOLATION_MENU = -1;
		IG_DATA::DELETE_INTERPOLATION = false;
		IG_DATA::INTERPOLATED_VIEW = -1;
		IG_DATA::NINT--;

		if (IG_DATA::NINT == 0)
			free(Interpolations);

		Interpolations = NULL;
	}

	if (IG_DATA::ADD_FIGURE != -1)
	{
		Interpolations[IG_DATA::INTERPOLATION_MENU]->addSurface(window.graphics, Figure[IG_DATA::ADD_FIGURE]);
		IG_DATA::ADD_FIGURE = -1;
	}

	if (IG_DATA::DELETE_FIGURE != -1)
	{
		Interpolations[IG_DATA::INTERPOLATION_MENU]->deleteSurface(window.graphics, IG_DATA::DELETE_FIGURE);
		IG_DATA::DELETE_FIGURE = -1;
	}

}

void Fourier::doFrame()
{
	eventManager();

	//	Update objects

	window.setTitle("Fourier  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	window.graphics.updatePerspective(observer, center, scale);

	if (IG_DATA::INTERPOLATED_VIEW != -1)
	{
		static float t = 0.f;
		t += 0.01f;
		Interpolations[IG_DATA::INTERPOLATED_VIEW]->updateInterpolation(window.graphics, t);
		Interpolations[IG_DATA::INTERPOLATED_VIEW]->updateRotation(window.graphics, rotation);
		window.graphics.clearBuffer(Color::Black);
		Interpolations[IG_DATA::INTERPOLATED_VIEW]->Draw(window.graphics);
		IG_Fourier::render();
		window.graphics.pushFrame();
		return;
	}

	INTERPRET_FIGURE_VIEW(IG_DATA::FIGURE_VIEW, updateRotation(window.graphics, rotation));

	if(IG_DATA::DOUBLE_VIEW)
		{INTERPRET_FIGURE_VIEW(IG_DATA::SECOND_VIEW, updateRotation(window.graphics, rotation));}

	//	Rendering

	window.graphics.clearBuffer(Color::Black);

	INTERPRET_FIGURE_VIEW(IG_DATA::FIGURE_VIEW, Draw(window.graphics));

	if(IG_DATA::DOUBLE_VIEW)
		{INTERPRET_FIGURE_VIEW(IG_DATA::SECOND_VIEW, Draw(window.graphics));}

	if (IG_DATA::CURVES)
		{INTERPRET_FIGURE_VIEW_NT(IG_DATA::FIGURE_VIEW, DrawCurves(window.graphics));}

	if(IG_DATA::CURVES && IG_DATA::DOUBLE_VIEW)
		{INTERPRET_FIGURE_VIEW_NT(IG_DATA::SECOND_VIEW, DrawCurves(window.graphics));}


	//	ImGui crap

	IG_Fourier::render();

	//	Push the frame to the scriin

	window.graphics.pushFrame();
}

//  Threading utilities

void createPlotAsync(Graphics* gfx, Polihedron* dataplot, const void** extractedFigure, bool* done, std::mutex* mtx)
{

	const Vector3f* vertexs = (Vector3f*)extractedFigure[0];
	const Vector3i* triangles = (Vector3i*)extractedFigure[1];
	int numT = ((int*)extractedFigure[2])[0];
	int numV = ((int*)extractedFigure[2])[1];

	dataplot->create(*gfx, vertexs, triangles, numT, nullptr, false, true, true, mtx);
	*done = true;
}

void createFigureAsync(Graphics* gfx, FourierSurface* figure, FourierSurface::Coefficient** coef, unsigned int ncoef, bool* done, std::mutex* mtx, bool* begin)
{
	while (begin && !(*begin))
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	*begin = false;

	// Crea la figura
	figure->create(*gfx, *coef, ncoef, mtx);
	*done = true;
}

void calculateCoefficientsAsync(FourierSurface::Coefficient** coef, const void** extractedFigure, unsigned int maxL, bool* done)
{
	*coef = FourierSurface::FileManager::calculateCoefficients(extractedFigure, maxL);
	*done = true;
}
