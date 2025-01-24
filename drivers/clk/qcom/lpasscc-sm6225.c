// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2022, 2023 Linaro Limited
 */

#include <linux/clk-provider.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#include <dt-bindings/clock/qcom,sm6225-lpasscc.h>

#include "common.h"
#include "reset.h"

static const struct qcom_reset_map lpass_audiocc_sm6225_resets[] = {
	[LPASS_AUDIO_SWR_RX_CGCR] =  { .reg = 0x98, .bit = 1, .udelay = 500 },
};

static struct regmap_config lpass_audiocc_sm6225_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
	.name = "lpass-audio-csr",
	.max_register = 0x1000,
};

static const struct qcom_cc_desc lpass_audiocc_sm6225_reset_desc = {
	.config = &lpass_audiocc_sm6225_regmap_config,
	.resets = lpass_audiocc_sm6225_resets,
	.num_resets = ARRAY_SIZE(lpass_audiocc_sm6225_resets),
};

static const struct qcom_reset_map lpasscc_sm6225_resets[] = {
	[LPASS_SWR_TX_CONFIG_CGCR] = { .reg = 0x100, .bit = 1, .udelay = 500 },
};

static struct regmap_config lpasscc_sm6225_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
	.name = "lpass-tcsr",
	.max_register = 0x1000,
};

static const struct qcom_cc_desc lpasscc_sm6225_reset_desc = {
	.config = &lpasscc_sm6225_regmap_config,
	.resets = lpasscc_sm6225_resets,
	.num_resets = ARRAY_SIZE(lpasscc_sm6225_resets),
};

static const struct of_device_id lpasscc_sm6225_match_table[] = {
	{
		.compatible = "qcom,sm6225-lpassaudiocc",
		.data = &lpass_audiocc_sm6225_reset_desc,
	}, {
		.compatible = "qcom,sm6225-lpasscc",
		.data = &lpasscc_sm6225_reset_desc,
	},
	{ },
};
MODULE_DEVICE_TABLE(of, lpasscc_sm6225_match_table);

static int lpasscc_sm6225_probe(struct platform_device *pdev)
{
	const struct qcom_cc_desc *desc = of_device_get_match_data(&pdev->dev);

	return qcom_cc_probe_by_index(pdev, 0, desc);
}

static struct platform_driver lpasscc_sm6225_driver = {
	.probe = lpasscc_sm6225_probe,
	.driver = {
		.name = "lpasscc-sm6225",
		.of_match_table = lpasscc_sm6225_match_table,
	},
};

module_platform_driver(lpasscc_sm6225_driver);

MODULE_DESCRIPTION("QTI LPASSCC SM6225 Driver");
MODULE_LICENSE("GPL");
