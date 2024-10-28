#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


static uint32_t s_MapWidth = 29;
static const char* s_Tilemap =
								"pppppppppppppp04|/Ppppp34/;aP"
								"pppppppppppppp04|/Ppppp34/;aP"
								"pppppppppppppp04|/Ppppp34/;aP"
								"pppppppppppppp04|/Ppppp34/;aP"
								"pppppppppppppp0Z&^Ppppp34/+1p"
								"%%%%%%%%%%%%%%$4|/@%%%%$CV@%%"
								";9;;;;;9;;;;;9;#*!;;;;9;#!;;;"
								"7877*77877777877777777877*777"
								"5z56|-5z55555z55555555z56|*55"
								"1124|/+================)4|/*4"
								"..3Z&^Ppppppppppppppppp0Z&^c4"
								"..34|/Ppppppppppppppppp04|/*4"
								"rr34|/Ppppppppppppppppp04|/*4"
								"GGR4|/Ppppppppppppppppp04|/*4"
								"GGR4|/Ppppppppppppppppp04|/*4"
								"GGR4|/Ppppppppppppppppp04|/*4"
								"GGR4|/Ppppppppppppppppp04|/c4";

static const char* s_Decor =
								"KIijKIijK 123F              v"
								"U___u___uA*()F              v"
								"ooooooooOaYy!F              v"
								"U---u---u @&^               v"
								"       N  #$%               V"
								"s                          fJ"
								"S EeRrRr                     "
								"    DdDd                     "
								"   Rr         Rr          Ee "
								"                             "
								"    T  M        ;::::[+      "
								"       H,78889, W]{}W]       "
								"     l h.45556. wkkkkm       "
								" g L T  .12223.tY!kkY!       "
								" G    P /*((()/Q@^kk@^       "
								"   B B  Yyyyyy!p@^kk@^       "
								" C      #$$$$$% @~yy`^       ";



							


Sandbox2D::Sandbox2D() 
	:Layer("Render Square and Triangle"), m_orthoCamController(1280.0f / 720.0f) {}

void Sandbox2D:: OnAttach() {
	FG_PROFILE_FUNCTION();
	m_Texture2D = Forge::Texture2D::Create("assets/Textures/CheckerBoard.png");
	m_SpriteSheet = Forge::Texture2D::Create("assets/Textures/tilemap_packed.png");
	//Grass
	m_mapTile['G'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, {1,16}, {16,16}, {1,1});

	//Pavement
	m_mapTile['R'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 15,16 }, { 16,16 }, { 1,1 });
	m_mapTile['r'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,17 }, { 16,16 }, { 1,1 });
	m_mapTile['.'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,16 }, { 16,16 }, { 1,1 });
	m_mapTile['1'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,15 }, { 16,16 }, { 1,1 });
	m_mapTile['2'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,15 }, { 16,16 }, { 1,1 });
	m_mapTile['3'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,16 }, { 16,16 }, { 1,1 });
	m_mapTile['P'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 8,16 }, { 16,16 }, { 1,1 });
	m_mapTile['p'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,16 }, { 16,16 }, { 1,1 });
	m_mapTile['+'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 8,15 }, { 16,16 }, { 1,1 });
	m_mapTile['='] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,15 }, { 16,16 }, { 1,1 });
	m_mapTile[')'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,15 }, { 16,16 }, { 1,1 });
	m_mapTile['0'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,16 }, { 16,16 }, { 1,1 });
	m_mapTile['%'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,17 }, { 16,16 }, { 1,1 });
	m_mapTile['$'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,17 }, { 16,16 }, { 1,1 });
	m_mapTile['@'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 8,17 }, { 16,16 }, { 1,1 });


	//Road
	m_mapTile['4'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 2,0 }, { 16,16 }, { 1,1 });
	m_mapTile['5'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 1,2 }, { 16,16 }, { 1,1 });
	m_mapTile['6'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 8,0 }, { 16,16 }, { 1,1 });
	m_mapTile['#'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 8,1 }, { 16,16 }, { 1,1 });
	m_mapTile['7'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 1,1 }, { 16,16 }, { 1,1 });
	m_mapTile[';'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 1,0 }, { 16,16 }, { 1,1 });
	m_mapTile['|'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 3,0 }, { 16,16 }, { 1,1 });
	m_mapTile['*'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,0 }, { 16,16 }, { 1,1 });
	m_mapTile['/'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 4,0 }, { 16,16 }, { 1,1 });
	m_mapTile['-'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 7,0 }, { 16,16 }, { 1,1 });
	m_mapTile['!'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 7,1 }, { 16,16 }, { 1,1 });
	m_mapTile['c'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,0 }, { 16,16 }, { 1,1 });
	m_mapTile['a'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,1 }, { 16,16 }, { 1,1 });
	m_mapTile['C'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 5,1 }, { 16,16 }, { 1,1 });
	m_mapTile['V'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 6,1 }, { 16,16 }, { 1,1 });

	//Zebra Crossing
	m_mapTile['z'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 0,2 }, { 16,16 }, { 1,1 });
	m_mapTile['8'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 0,1 }, { 16,16 }, { 1,1 });
	m_mapTile['9'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 0,0 }, { 16,16 }, { 1,1 });
	m_mapTile['Z'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 2,1 }, { 16,16 }, { 1,1 });
	m_mapTile['&'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 3,1 }, { 16,16 }, { 1,1 });
	m_mapTile['^'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 4,1 }, { 16,16 }, { 1,1 });

	//People
	m_mapDecor['C'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 26,7 }, { 16,16 }, { 1,1 });
	m_mapDecor['P'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 24,8 }, { 16,16 }, { 1,1 });
	m_mapDecor['+'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 26,16 }, { 16,16 }, { 1,1 });
	m_mapDecor['J'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 23,14 }, { 16,16 }, { 1,1 });
	//Lights
	m_mapDecor['G'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 7,11 }, { 16,16 }, { 1,1 });
	m_mapDecor['g'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 7,10 }, { 16,16 }, { 1,1 });
	m_mapDecor['S'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 3,11 }, { 16,16 }, { 1,1 });
	m_mapDecor['s'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 3,10 }, { 16,16 }, { 1,1 });
	m_mapDecor['H'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 2,10 }, { 16,16 }, { 1,1 });
	m_mapDecor['h'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 2,11 }, { 16,16 }, { 1,1 });

	//Road signs
	m_mapDecor['B'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 5,9 }, { 16,16 }, { 1,1 });
	m_mapDecor['L'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 0,6 }, { 16,16 }, { 1,1 });
	m_mapDecor['l'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 0,7 }, { 16,16 }, { 1,1 });
	m_mapDecor['T'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,6 }, { 16,16 }, { 1,1 });
	
	//Cars
	m_mapDecor['E'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 21,0 }, { 16,16 }, { 1,1 });
	m_mapDecor['e'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 22,0 }, { 16,16 }, { 1,1 });
	m_mapDecor['R'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 18,2 }, { 16,16 }, { 1,1 });
	m_mapDecor['r'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 19,2 }, { 16,16 }, { 1,1 });
	m_mapDecor['D'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 18,3 }, { 16,16 }, { 1,1 });
	m_mapDecor['d'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 19,3 }, { 16,16 }, { 1,1 });

	//Buildings
	m_mapDecor['U'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 13,12 }, { 16,16 }, { 1,1 });
	m_mapDecor['u'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 14,12 }, { 16,16 }, { 1,1 });
	m_mapDecor['-'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,14 }, { 16,16 }, { 1,1 });
	m_mapDecor['o'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,13 }, { 16,16 }, { 1,1 });
	m_mapDecor['_'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,12 }, { 16,16 }, { 1,1 });
	m_mapDecor['O'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,13 }, { 16,16 }, { 1,1 });
	m_mapDecor['K'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 16,13 }, { 16,16 }, { 1,1 });
	m_mapDecor['I'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 17,13 }, { 16,16 }, { 1,1 });
	m_mapDecor['i'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 18,13 }, { 16,16 }, { 1,1 });
	m_mapDecor['j'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 19,13 }, { 16,16 }, { 1,1 });
	m_mapDecor['/'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 16,17 }, { 16,16 }, { 1,1 });
	m_mapDecor['.'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 16,16 }, { 16,16 }, { 1,1 });
	m_mapDecor[','] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 16,14 }, { 16,16 }, { 1,1 });
	m_mapDecor['*'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 17,17 }, { 16,16 }, { 1,1 });
	m_mapDecor['('] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 18,17 }, { 16,16 }, { 1,1 });
	m_mapDecor[')'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 19,17}, {16,16}, {1,1});
	m_mapDecor['1'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 17,16 }, { 16,16 }, { 1,1 });
	m_mapDecor['2'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 18,16 }, { 16,16 }, { 1,1 });
	m_mapDecor['3'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 19,16 }, { 16,16 }, { 1,1 });
	m_mapDecor['4'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 17,15 }, { 16,16 }, { 1,1 });
	m_mapDecor['5'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 18,15 }, { 16,16 }, { 1,1 });
	m_mapDecor['6'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 19,15 }, { 16,16 }, { 1,1 });
	m_mapDecor['7'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 17,14 }, { 16,16 }, { 1,1 });
	m_mapDecor['8'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 18,14 }, { 16,16 }, { 1,1 });
	m_mapDecor['9'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 19,14 }, { 16,16 }, { 1,1 });
	m_mapDecor['Y'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 0,12 }, { 16,16 }, { 1,1 });
	m_mapDecor['y'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 1,12 }, { 16,16 }, { 1,1 });
	m_mapDecor['!'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 2,12 }, { 16,16 }, { 1,1 });
	m_mapDecor['@'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 0,13 }, { 16,16 }, { 1,1 });
	m_mapDecor['#'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 0,14 }, { 16,16 }, { 1,1 });
	m_mapDecor['$'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 1,14 }, { 16,16 }, { 1,1 });
	m_mapDecor['%'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 2,14 }, { 16,16 }, { 1,1 });
	m_mapDecor['^'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 2,13 }, { 16,16 }, { 1,1 });
	m_mapDecor['&'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 1,13 }, { 16,16 }, { 1,1 });
	m_mapDecor['W'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 8,3 }, { 16,16 }, { 1,1 });
	m_mapDecor['{'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 7,2 }, { 16,16 }, { 1,1 });
	m_mapDecor['}'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 8,2 }, { 16,16 }, { 1,1 });
	m_mapDecor[']'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,3 }, { 16,16 }, { 1,1 });
	m_mapDecor['w'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 17,11 }, { 16,16 }, { 1,1 });
	m_mapDecor['k'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 18,11 }, { 16,16 }, { 1,1 });
	m_mapDecor['m'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 19,11 }, { 16,16 }, { 1,1 });
	m_mapDecor[']'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 10,3 }, { 16,16 }, { 1,1 });
	m_mapDecor['~'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 5,14 }, { 16,16 }, { 1,1 });
	m_mapDecor['`'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 6,14 }, { 16,16 }, { 1,1 });
	

	//Trees
	m_mapDecor['N'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 16,9 }, { 16,16 }, { 1,1 });

	//Misc
	m_mapDecor['M'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 8,6 }, { 16,16 }, { 1,1 });
	m_mapDecor['Q'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 4,3 }, { 16,16 }, { 1,1 });
	m_mapDecor['t'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 9,7 }, { 16,16 }, { 1,1 });
	m_mapDecor['p'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 4,8 }, { 16,16 }, { 1,1 });
	m_mapDecor[';'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 11,15 }, { 16,16 }, { 1,1 });
	m_mapDecor[':'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 12,15 }, { 16,16 }, { 1,1 });
	m_mapDecor['['] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 13,15 }, { 16,16 }, { 1,1 });
	m_mapDecor['A'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 3,8 }, { 16,16 }, { 1,1 });
	m_mapDecor['a'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 1,8 }, { 16,16 }, { 1,1 });
	m_mapDecor['F'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 2,3 }, { 16,16 }, { 1,1 });
	m_mapDecor['f'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 7,7 }, { 16,16 }, { 1,1 });
	m_mapDecor['V'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 1,10 }, { 16,16 }, { 1,1 });
	m_mapDecor['v'] = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 2,6 }, { 16,16 }, { 1,1 });

	m_Error = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 11,8 }, { 16,16 }, { 1,1 });

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.Lifetime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_orthoCamController.SetZoomLevel(9.0f);

	m_mapWidth = s_MapWidth;
	m_mapHeight = strlen(s_Tilemap) / m_mapWidth;
	FG_INFO("Map Width:{0},Map Height:{1}", m_mapWidth, m_mapHeight);

}
void Sandbox2D:: OnDetach() {
	FG_PROFILE_FUNCTION();
}

static float rot = 0.0f;
void Sandbox2D:: OnUpdate(Forge::Timestep time) {

	FG_PROFILE_FUNCTION();
	rot += 45 * time;

	m_orthoCamController.OnUpdate(time);

	{
		FG_PROFILE_SCOPE("Renderer Setup");

		Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Forge::RenderCommands::Clear();
	}

	Forge::Renderer2D::ResetStats();

#if 0
	Forge::Renderer2D::BeginScene(m_orthoCamController.GetCamera());

	Forge::Renderer2D::DrawQuad({ 0.5f,0.0f,0.0f }, glm::vec2(1.0f), { m_SquareColor, 1.0f });
	Forge::Renderer2D::DrawRotatedQuad(glm::vec3(5.0f,0.0f,0.0f), glm::vec2(1.0f), 45.0f, { m_SquareColor, 1.0f });
	Forge::Renderer2D::DrawQuad({ 1.5f,-0.5f,0.0f }, glm::vec2(0.5f, 0.8f), { 0.8f,0.2f,0.2f, 1.0f });
	Forge::Renderer2D::DrawQuad({ -5.0f,-5.0f,-0.1f }, glm::vec2(10.0f,10.0f), m_Texture2D, 10);
	Forge::Renderer2D::DrawRotatedQuad({ 0.0f,0.0f,0.0f }, glm::vec2(1.0f,1.0f),rot, m_Texture2D,{0.2,0.8,0.2,1.0f});

	Forge::Renderer2D::EndScene();

	Forge::Renderer2D::BeginScene(m_orthoCamController.GetCamera());
	 
	for (float y = -5.0f; y < 5.0f; y += 0.1f) {
		for (float x = -5.0f; x < 5.0f; x += 0.1f) {
			glm::vec4 color = { (x + 5.0f) / 10, 0.3, (y + 5.0f )/ 10,0.5f };
			Forge::Renderer2D::DrawQuad({ x,y,0.0f }, glm::vec2(0.45f, 0.45f), color);
		}
	}

	Forge::Renderer2D::EndScene(); 
#endif


	if (Forge::Input::IsMouseButtonPressed(FG_MOUSE_BUTTON_1))
	{
		auto [x, y] = Forge::Input::GetMousePosition();
		auto width = Forge::Application::Get().GetWindow().GetWidth();
		auto height = Forge::Application::Get().GetWindow().GetHeight();

		auto bounds = m_orthoCamController.GetBounds();
		auto pos = m_orthoCamController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(time);
	m_ParticleSystem.OnRender(m_orthoCamController.GetCamera());


	Forge::Renderer2D::BeginScene(m_orthoCamController.GetCamera());

	for (uint32_t y = 0; y < m_mapHeight; y++) {
		for (uint32_t x = 0; x < m_mapWidth; x++) {
			char tileType = s_Tilemap[x + y * m_mapWidth];

			if (tileType == ' ')
				continue;

			Forge::Ref<Forge::SubTextures2D> texture;
			if (m_mapTile.find(tileType) != m_mapTile.end()) {
				texture = m_mapTile[tileType];
			}
			else {
				texture = m_Error;
			}

			Forge::Renderer2D::DrawQuad({ x - (m_mapWidth/2.0f),y - (m_mapHeight /2.0f) ,0.5f }, glm::vec2(1.0f, 1.0f), texture);
		}
	}

	for (uint32_t y = 0; y < m_mapHeight; y++) {
		for (uint32_t x = 0; x < m_mapWidth; x++) {
			char tileType = s_Decor[x + y * m_mapWidth];

			if (tileType == ' ')
				continue;

			Forge::Ref<Forge::SubTextures2D> texture;
			if (m_mapDecor.find(tileType) != m_mapDecor.end()) {
				texture = m_mapDecor[tileType];
			}
			else {
				texture = m_Error;
			}

			Forge::Renderer2D::DrawQuad({ x - (m_mapWidth / 2.0f),y - (m_mapHeight / 2.0f) ,1.0f }, glm::vec2(1.0f, 1.0f), texture);
		}
	}
	
	Forge::Renderer2D::EndScene();
	
}
void Sandbox2D:: OnImGuiRender() {

	FG_PROFILE_FUNCTION();

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static bool dockspace_open = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::MenuItem("Exit")) { Forge::Application::Get().Close(); }
			ImGui::Separator();

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");
	//ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit3("Begin Color", glm::value_ptr(m_Particle.ColorBegin));
	ImGui::ColorEdit3("End Color", glm::value_ptr(m_Particle.ColorEnd));
	ImGui::Text("RendererStats: ");
	ImGui::Text("    Draw Calls: %d", Forge::Renderer2D::GetStat().DrawCalls);
	ImGui::Text("    NumQuads: %d", Forge::Renderer2D::GetStat().NumQuads);
	ImGui::Text("    Num Vertices: %d", Forge::Renderer2D::GetStat().GetNumVertices());
	ImGui::Text("    Num Indices: %d", Forge::Renderer2D::GetStat().GetNumIndices());

	uint32_t textureID = m_Texture2D->GetRendererID();
	ImGui::Image((void*)textureID, ImVec2{256,256});
	ImGui::End();

	ImGui::End();

}
void Sandbox2D:: OnEvent(Forge::Event& e) {
	m_orthoCamController.OnEvent(e);
} 