#!/usr/bin/env python3
"""Build an SVG plot from hash_collision_data.csv (no third-party deps)."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path


def read_series(path: Path) -> tuple[list[int], dict[str, list[int]]]:
	with path.open(newline="", encoding="utf-8") as f:
		reader = csv.reader(f)
		header = next(reader)
		if not header or header[0].strip().upper() != "N":
			raise SystemExit(f"Expected first column 'N', got {header!r}")
		names = [h.strip() for h in header[1:]]
		ns: list[int] = []
		data: dict[str, list[int]] = {name: [] for name in names}
		for row in reader:
			if not row or not row[0].strip():
				continue
			ns.append(int(row[0]))
			for i, name in enumerate(names):
				v = int(row[i + 1]) if i + 1 < len(row) and row[i + 1].strip() else 0
				data[name].append(v)
	return ns, data


def svg_escape(text: str) -> str:
	return (
		text.replace("&", "&amp;")
		.replace("<", "&lt;")
		.replace(">", "&gt;")
		.replace('"', "&quot;")
	)


def build_svg(
	ns: list[int],
	series: dict[str, list[int]],
	width: int = 900,
	height: int = 520,
) -> str:
	margin_l, margin_r, margin_t, margin_b = 72, 40, 48, 56
	plot_w = width - margin_l - margin_r
	plot_h = height - margin_t - margin_b

	x_max = max(ns) if ns else 1
	y_max = 1
	for vals in series.values():
		if vals:
			y_max = max(y_max, max(vals))
	y_max = max(y_max, 1)

	def sx(x: int) -> float:
		return margin_l + (x / x_max) * plot_w

	def sy(y: int) -> float:
		return margin_t + plot_h - (y / y_max) * plot_h

	colors = [
		"#1f77b4",
		"#ff7f0e",
		"#2ca02c",
		"#d62728",
		"#9467bd",
		"#8c564b",
		"#e377c2",
		"#7f7f7f",
		"#bcbd22",
	]

	lines: list[str] = [
		f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
		'font-family="system-ui,Segoe UI,sans-serif" font-size="13">',
		f'<rect width="100%" height="100%" fill="#fafafa"/>',
		f'<text x="{width // 2}" y="28" text-anchor="middle" font-size="17" font-weight="600">'
		f"{svg_escape('Hash collisions vs number of strings (Partow, 32-bit)')}</text>",
		f'<rect x="{margin_l}" y="{margin_t}" width="{plot_w}" height="{plot_h}" '
		'fill="#fff" stroke="#ccc"/>',
	]

	for frac in (0.25, 0.5, 0.75):
		gy = margin_t + plot_h * (1.0 - frac)
		yv = int(round(y_max * frac))
		lines.append(
			f'<line x1="{margin_l}" y1="{gy:.1f}" x2="{margin_l + plot_w}" y2="{gy:.1f}" stroke="#eee"/>'
		)
		lines.append(
			f'<text x="{margin_l - 8}" y="{gy + 4}" text-anchor="end" fill="#555">{yv}</text>'
		)

	lines.append(
		f'<text x="{margin_l + plot_w // 2}" y="{height - 14}" text-anchor="middle" fill="#333">'
		f"{svg_escape('N (strings)')}</text>"
	)
	lines.append(
		f'<text transform="translate(22,{margin_t + plot_h // 2}) rotate(-90)" '
		f'text-anchor="middle" fill="#333">{svg_escape("Pairwise collisions")}</text>'
	)

	for i, n in enumerate(ns):
		if i == 0 or i == len(ns) - 1 or i == len(ns) // 2:
			x = sx(n)
			lines.append(
				f'<line x1="{x:.1f}" y1="{margin_t + plot_h}" x2="{x:.1f}" y2="{margin_t + plot_h + 5}" stroke="#333"/>'
			)
			lines.append(
				f'<text x="{x:.1f}" y="{margin_t + plot_h + 20}" text-anchor="middle" fill="#555">{n}</text>'
			)

	for idx, (name, vals) in enumerate(series.items()):
		if len(vals) != len(ns):
			continue
		color = colors[idx % len(colors)]
		points = " ".join(f"{sx(ns[i]):.1f},{sy(vals[i]):.1f}" for i in range(len(ns)))
		lines.append(
			f'<polyline fill="none" stroke="{color}" stroke-width="2" '
			f'points="{points}" stroke-linejoin="round" stroke-linecap="round"/>'
		)

	legend_x = margin_l + plot_w - 8
	legend_y = margin_t + 14
	for idx, name in enumerate(series.keys()):
		color = colors[idx % len(colors)]
		cy = legend_y + idx * 18
		lines.append(f'<line x1="{legend_x - 80}" y1="{cy}" x2="{legend_x - 60}" y2="{cy}" stroke="{color}" stroke-width="3"/>')
		lines.append(
			f'<text x="{legend_x - 56}" y="{cy + 4}" fill="#222">{svg_escape(name)}</text>'
		)

	lines.append("</svg>")
	return "\n".join(lines)


def main() -> None:
	parser = argparse.ArgumentParser(description="Plot hash collision CSV to SVG.")
	parser.add_argument(
		"csv",
		type=Path,
		nargs="?",
		default=Path(__file__).resolve().parent / "hash_collision_data.csv",
	)
	parser.add_argument(
		"-o",
		"--output",
		type=Path,
		default=Path(__file__).resolve().parent / "hash_collisions.svg",
	)
	args = parser.parse_args()
	if not args.csv.is_file():
		raise SystemExit(f"CSV not found: {args.csv} (run ./10.05 first)")
	ns, series = read_series(args.csv)
	svg = build_svg(ns, series)
	args.output.write_text(svg, encoding="utf-8")
	print(f"Wrote {args.output}")


if __name__ == "__main__":
	main()
