/**
 * @author: github.com/velechva
 * 
 * This software is licensed under the GNU General Public License v3.
 * See LICENSE.md for more details
 */

#include <random>

#include "plugin.hpp"
#include "math.hpp"
#include "types.hpp"
#include "oscillator.hpp"

#define NUM_OSCILLATORS 4

/**
 * Widget switch to toggle waveform for an oscillator
 * 
 * @note for performance reasons, this should have the same order as @see{WavetableOscillator::Wavetable}
 */
struct WaveformPicker : SvgSwitch {
	WaveformPicker() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch/Sine.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch/Square.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch/Crazy-Sine.svg")));
	}
};

class Prizm : public Module {
public:
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
		configParam(A_WAVE_TOGGLE, 0.0, 5.0, 0.0, "Wave Toggle A");
		configParam(B_WAVE_TOGGLE, 0.0, 5.0, 0.0, "Wave Toggle B");
		configParam(C_WAVE_TOGGLE, 0.0, 5.0, 0.0, "Wave Toggle C");
		configParam(D_WAVE_TOGGLE, 0.0, 5.0, 0.0, "Wave Toggle D");

		this->m_oscillators[0].setWavetable(WavetableOscillator::Wavetable::SINE);
		this->m_oscillators[1].setWavetable(WavetableOscillator::Wavetable::SQUARE);
		this->m_oscillators[2].setWavetable(WavetableOscillator::Wavetable::CRAZY_SINE);
		this->m_oscillators[3].setWavetable(WavetableOscillator::Wavetable::FM_ONE);
	}

	/**
	 * Process a new sample
	 */
	void process(const ProcessArgs& args) override {
		accumulatePhase(args);

		for (size_t i = 0; i < 4; i++) {
			auto oscillator = m_oscillators[i];

			/**
			 * current un-normalized mix value = switch value + cv input value
			 */
			m_mixValues[i] = clamp(
				inputs[MOD_A_INPUT + i].getVoltage() + params[A_WAVE_MIX + i].getValue(),
				-5.0f, 5.0f
			);

			m_wavepoints[i] = oscillator.getNextWavepoint(m_phase, 0.5f);
		}

		normalize(m_mixValues, NUM_OSCILLATORS);

		for (size_t i = 0; i < NUM_OSCILLATORS; i++) {
			m_wavepoints[i] *= m_mixValues[i];
		}

		float output = tanh(pythagoras(m_wavepoints, NUM_OSCILLATORS));

		if (outputs[MAIN_OUTPUT].isConnected()) {
			outputs[MAIN_OUTPUT].setVoltage(5.0f * output);
		}
	}
private:
	float m_phase = 0.0f;

	float m_wavepoints [NUM_OSCILLATORS] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float m_mixValues  [NUM_OSCILLATORS] = { 0.0f, 0.0f, 0.0f, 0.0f };

	WavetableOscillator m_oscillators [4] = { 
		WavetableOscillator(), 
		WavetableOscillator(), 
		WavetableOscillator(), 
		WavetableOscillator() 
	};

	inline hz getFrequency() {
		float pitch = inputs[V_OCT_INPUT].getVoltage();

		return dsp::FREQ_C4 * std::pow(2.0f, pitch);
	}

	inline void accumulatePhase(const ProcessArgs& args) {
		m_phase += this->getFrequency() * args.sampleTime;

		if (m_phase >= 0.5f) {
			m_phase -= 1.f;
		}
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