#include "ShaderDemos.h"
#include "common.h"
#include "WiEngine.h"

namespace Shader {

    static void createButton(const char* label, wyLayer* layer, float x, float y, wyTargetSelector* ts) {
		wyNinePatchSprite* normal1 = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_normal")), wyr(DP(9), DP(7), DP(22), DP(28)));
		wyNinePatchSprite* pressed1 = wyNinePatchSprite::make(wyTexture2D::makePNG(RES("R.drawable.btn_pressed")), wyr(DP(9), DP(7), DP(22), DP(28)));
		normal1->setContentSize(DP(300), DP(44));
		pressed1->setContentSize(DP(300), DP(44));
        
		wyButton* button1 = wyButton::make(normal1, pressed1, NULL, NULL, NULL, ts);
		button1->setPosition(x, y);
        
		wyBitmapFont* font = wyBitmapFont::loadFont(RES("R.raw.bitmapfont"));
		wyBitmapFontLabel* label1 = wyBitmapFontLabel::make(font, label);
		label1->setColor(wyc3b(255, 255, 0));
		label1->setPosition(x, y);
        
		layer->addChildLocked(button1);
		layer->addChildLocked(label1);
	}
    
	/////////////////////////////////////////////////////////////////////////////////

	class wyAlphaTestTestLayer : public wyLayer, public wySliderCallback {
	private:
		wySprite* m_sprite;
		wySlider* m_slider;
		wyLabel* m_hint;

	public:
		wyAlphaTestTestLayer() {
			// load custom shader, id is 1
			wyShaderProgram* p = wyShaderManager::getInstance()->addProgram(1, RES("R.raw.shader_alpha_test_vsh"), RES("R.raw.shader_alpha_test_fsh"));
			p->addAttribute(wyShaderVariable::VEC4, wyAttribute::NAME[wyAttribute::POSITION], wyAttribute::POSITION);
			p->addAttribute(wyShaderVariable::VEC4, wyAttribute::NAME[wyAttribute::COLOR], wyAttribute::COLOR);
			p->addAttribute(wyShaderVariable::VEC2, wyAttribute::NAME[wyAttribute::TEXTURE], wyAttribute::TEXTURE);
			if(p->link()) {
				p->addUniform(wyShaderVariable::MAT4,
						wyUniform::NAME[wyUniform::WORLD_VIEW_PROJECTION_MATRIX],
						wyUniform::WORLD_VIEW_PROJECTION_MATRIX);
				p->addUniform(wyShaderVariable::TEXTURE_2D,
						wyUniform::NAME[wyUniform::TEXTURE_2D],
						wyUniform::TEXTURE_2D);
				p->addUniform(wyShaderVariable::FLOAT,
						"u_alpha",
						wyUniform::CUSTOM);
			}

			// create
			m_sprite = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.alpha_test")));
			m_sprite->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2 + DP(80));
			addChildLocked(m_sprite);

			// now get material of sprite and set to custom shader
			wyMaterial* m = m_sprite->getMaterial();
			m->getTechnique()->setProgram(1);

			// add an alpha parameter to material
			wyShaderVariable::Value v;
			v.f = 0;
			wyMaterialParameter* mp = wyMaterialParameter::make("u_alpha", v);
			m->addParameter(mp);

			// create a hint label
			m_hint = wyLabel::make("drag slider", SP(18));
			m_hint->setPosition(wyDevice::winWidth / 2, DP(80));
			addChildLocked(m_hint);

			// alpha value slider
			wySprite* bar = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.bar")));
			wySprite* thumb = wySprite::make(wyTexture2D::makePNG(RES("R.drawable.thumb")));
			m_slider = wySlider::make(NULL, bar, thumb);
			m_slider->setValue(0);
			m_slider->setMax(255);
			m_slider->setShowFullBar(true);
			m_slider->setPosition(wyDevice::winWidth / 2, DP(50));
			m_slider->setCallback(this);
			addChildLocked(m_slider);
		}

		virtual ~wyAlphaTestTestLayer() {
		}

		virtual void onSliderValueChanged(wySlider* slider) {
			wyMaterial* m = m_sprite->getMaterial();
			wyMaterialParameter* mp = m->getParameter("u_alpha");
			wyShaderVariable::Value v;
			v.f = slider->getValue() / 255.0f;
			mp->setValue(v);
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////

	class wyWavingFlagTestLayer : public wyLayer {
	private:
		float m_time;
		wySprite* m_flag;

	public:
		wyWavingFlagTestLayer() : m_time(0) {
			// load custom shader, id is 1
			wyShaderProgram* p = wyShaderManager::getInstance()->addProgram(1, RES("R.raw.shader_waving_flag_vsh"), RES("R.raw.shader_waving_flag_fsh"));
			p->addAttribute(wyShaderVariable::VEC4, wyAttribute::NAME[wyAttribute::POSITION], wyAttribute::POSITION);
			p->addAttribute(wyShaderVariable::VEC4, wyAttribute::NAME[wyAttribute::COLOR], wyAttribute::COLOR);
			p->addAttribute(wyShaderVariable::VEC2, wyAttribute::NAME[wyAttribute::TEXTURE], wyAttribute::TEXTURE);
			if(p->link()) {
				p->addUniform(wyShaderVariable::MAT4,
						wyUniform::NAME[wyUniform::WORLD_VIEW_PROJECTION_MATRIX],
						wyUniform::WORLD_VIEW_PROJECTION_MATRIX);
				p->addUniform(wyShaderVariable::TEXTURE_2D,
						wyUniform::NAME[wyUniform::TEXTURE_2D],
						wyUniform::TEXTURE_2D);
				p->addUniform(wyShaderVariable::FLOAT,
						"u_time",
						wyUniform::CUSTOM);
			}

			// create
			m_flag = wySprite::make(wyTexture2D::makeJPG(RES("R.drawable.usa_flag")));
			m_flag->setPosition(wyDevice::winWidth / 2, wyDevice::winHeight / 2);
			addChildLocked(m_flag);

			// now get material of sprite and set to custom shader
			wyMaterial* m = m_flag->getMaterial();
			m->getTechnique()->setProgram(1);

			// add an alpha parameter to material
			wyShaderVariable::Value v;
			v.f = m_time;
			wyMaterialParameter* mp = wyMaterialParameter::make("u_time", v);
			m->addParameter(mp);

			// use grid 3d mesh for this sprite
			wyGrid3D* grid = wyGrid3D::make(m_flag->getWidth(), m_flag->getHeight(), m_flag->getWidth(), 1);
			m_flag->replaceMesh(grid);

			// schedule a time to update uniform
			scheduleLocked(wyTimer::make(wyTargetSelector::make(this, SEL(wyWavingFlagTestLayer::onUpdateTimeUniform))));
		}

		virtual ~wyWavingFlagTestLayer() {
		}

		void onUpdateTimeUniform(wyTargetSelector* ts) {
			m_time += ts->getDelta();
			wyMaterial* m = m_flag->getMaterial();
			wyMaterialParameter* mp = m->getParameter("u_time");
			wyShaderVariable::Value v;
			v.f = m_time;
			mp->setValue(v);
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////
}

using namespace Shader;

DEMO_ENTRY_IMPL(shader, AlphaTestTest);
DEMO_ENTRY_IMPL(shader, WavingFlagTest);