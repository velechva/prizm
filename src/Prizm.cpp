
#include "plugin.hpp"

struct Prizm : Module {
	enum ParamIds {
		PITCH_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
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
	}

	float phase = 0.f;

	float mktri(float phase) {
		return std::abs(phase);
	}

	void process(const ProcessArgs& args) override {
		float pitch = clamp(params[PITCH_PARAM].getValue(), -4.f, 4.f);
		float freq = dsp::FREQ_C4 * std::pow(2.f, 0);

		float mix = clamp(pitch, -1.f, 1.f);

		// Accumulate the phase
		phase += freq * args.sampleTime;
		if (phase >= 0.5f)
			phase -= 1.f;

		// Compute the sine output
		float sine = std::sin(2.f * M_PI * phase);
		float sqr = phase > 0 ? 1 : -1;
		float out = std::pow(
			mix * std::pow(sine, 2) + (1 - mix) * std::pow(sqr, 2),
			0.5f
		);
		// Audio signals are typically +/-5V
		// https://vcvrack.com/manual/VoltageStandards
		outputs[MAIN_OUTPUT].setVoltage(5.f * out);
	}
};


struct PrizmWidget : ModuleWidget {
	PrizmWidget(Prizm* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panel.svg")));

		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

		addParam(createParam<RoundLargeBlackKnob>(Vec(47, 61), module, Prizm::PITCH_PARAM));

		addOutput(createOutput<PJ301MPort>(Vec(11, 320), module, Prizm::MAIN_OUTPUT));
	}
};

Model* modelPrizm = createModel<Prizm, PrizmWidget>("Prizm");
