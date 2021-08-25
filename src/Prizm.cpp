/**
 * @author: github.com/velechva
 * 
 * This software is licensed under the GNU General Public License v3.
 * See LICENSE.md for more details
 */

#include <random>

#include "plugin.hpp"
#include "math.hpp"
struct Prizm : Module {
	enum ParamIds {
		PITCH_PARAM,
		A_WAVE_TOGGLE,
		B_WAVE_TOGGLE,
		C_WAVE_TOGGLE,
		D_WAVE_TOGGLE,
		A_WAVE_MIX,
		B_WAVE_MIX,
		C_WAVE_MIX,
		D_WAVE_MIX,
		KEY_TRACK_AMOUNT,
		NUM_PARAMS
	};
	enum InputIds {
		MOD_A_INPUT,
		MOD_B_INPUT,
		MOD_C_INPUT,
		MOD_D_INPUT,
		V_OCT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		MAIN_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(PHASE_LIGHT, 3),
		NUM_LIGHTS
	};

	Prizm() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(A_WAVE_TOGGLE, 0.0, 1.0, 0.0, "Wave Toggle A");
		configParam(B_WAVE_TOGGLE, 0.0, 1.0, 0.0, "Wave Toggle B");
		configParam(C_WAVE_TOGGLE, 0.0, 1.0, 0.0, "Wave Toggle C");
		configParam(D_WAVE_TOGGLE, 0.0, 1.0, 0.0, "Wave Toggle D");
	}

	float phase = 0.f;

	void process(const ProcessArgs& args) override {
		float pitch = inputs[V_OCT_INPUT].getVoltage();
		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);

		// Accumulate the phase
		phase += freq * args.sampleTime;
		if (phase >= 0.5f)
			phase -= 1.f;


		// Compute the sine output
		float sineVal = sineWave(phase);
		float sqreVal = squareWave(phase);
		float subVal = subWave(phase);
		float crazyVal = crazierWave(phase);

		// TODO normalize
		// TODO add normalize toggle switch
		float mix_a = clamp(params[A_WAVE_MIX].getValue() + inputs[MOD_A_INPUT].getVoltage(), 0.0, 1.0);
		float mix_b = clamp(params[B_WAVE_MIX].getValue() + inputs[MOD_B_INPUT].getVoltage(), 0.0, 1.0);
		float mix_c = clamp(params[C_WAVE_MIX].getValue() + inputs[MOD_C_INPUT].getVoltage(), 0.0, 1.0);
		float mix_d = clamp(params[D_WAVE_MIX].getValue() + inputs[MOD_D_INPUT].getVoltage(), 0.0, 1.0);

		float out = pythagoras(mix_a * sineVal, mix_b * sqreVal, mix_c * subVal, mix_d * crazyVal);

		if (outputs[MAIN_OUTPUT].isConnected()) {
			outputs[MAIN_OUTPUT].setVoltage(5.f * out);
		}
	}
};

struct WaveformPicker : SVGSwitch {
	WaveformPicker() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch-Sin.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch-Sqr.svg")));
	}
};

struct PrizmWidget : ModuleWidget {
	PrizmWidget(Prizm* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Prizm.svg")));

		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

		addParam(createParam<RoundBlackKnob>(Vec(11, 55), module, Prizm::A_WAVE_MIX));
		addParam(createParam<RoundBlackKnob>(Vec(172, 55), module, Prizm::B_WAVE_MIX));
		addParam(createParam<RoundBlackKnob>(Vec(11, 220), module, Prizm::C_WAVE_MIX));
		addParam(createParam<RoundBlackKnob>(Vec(172, 220), module, Prizm::D_WAVE_MIX));

		addParam(createParam<WaveformPicker>(Vec(3, 91), module, Prizm::A_WAVE_TOGGLE));
		addParam(createParam<WaveformPicker>(Vec(148, 91), module, Prizm::B_WAVE_TOGGLE));
		addParam(createParam<WaveformPicker>(Vec(3, 256), module, Prizm::C_WAVE_TOGGLE));
		addParam(createParam<WaveformPicker>(Vec(148, 256), module, Prizm::D_WAVE_TOGGLE));

		addInput(createInput<PJ301MPort>(Vec(12, 310), module, Prizm::V_OCT_INPUT));

		addInput(createInput<PJ301MPort>(Vec(11, 125), module, Prizm::MOD_A_INPUT));
		addInput(createInput<PJ301MPort>(Vec(172, 125), module, Prizm::MOD_B_INPUT));
		addInput(createInput<PJ301MPort>(Vec(11, 182), module, Prizm::MOD_C_INPUT));
		addInput(createInput<PJ301MPort>(Vec(172, 182), module, Prizm::MOD_D_INPUT));

		addOutput(createOutput<PJ301MPort>(Vec(175, 310), module, Prizm::MAIN_OUTPUT));
	}
};

Model* modelPrizm = createModel<Prizm, PrizmWidget>("Prizm");
