document.addEventListener('DOMContentLoaded', () => {
	const footer = document.querySelector('.footer');

	if (!footer) {
		return;
	}

	let mousePhase = 0;
	let mouseBoost = 0;
	let scrollBoost = 0;

	const basePoints = [
		[0, 100],
		[0, 49],
		[10, 53],
		[20, 48],
		[30, 52],
		[40, 47],
		[50, 51],
		[60, 48],
		[70, 52],
		[80, 49],
		[90, 53],
		[100, 50],
		[100, 100]
	];

	const animatedIndices = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11];

	// El pie de página responde al ratón y al scroll con una deformación suave.
	window.addEventListener('mousemove', (event) => {
		const width = window.innerWidth || 1;
		const height = window.innerHeight || 1;
		mousePhase = (event.clientX / width) * Math.PI * 2;
		mouseBoost = 1 + ((width - event.clientY) / height) * 1.2;
	});

	window.addEventListener('scroll', () => {
		scrollBoost = 1.4;
	});

	// La animación recalcula el clip-path en cada frame para crear el efecto ondulante.
	function animateClipPath(time) {
		mouseBoost *= 0.96;
		scrollBoost *= 0.92;

		const points = basePoints.map(([x, y]) => [x, y]);
		const intensity = 1 + mouseBoost * 0.9 + scrollBoost * 1.2;

		animatedIndices.forEach((index, i) => {
			const waveA = Math.sin(time * 0.0018 + i * 0.8 + mousePhase) * 2.4;
			const waveB = Math.cos(time * 0.0012 + i * 1.1 + mousePhase * 0.6) * 1.4;
			points[index][1] = basePoints[index][1] + (waveA + waveB) * intensity;
		});

		footer.style.clipPath = `polygon(${points.map(([x, y]) => `${x}% ${y}%`).join(', ')})`;
		requestAnimationFrame(animateClipPath);
	}

	requestAnimationFrame(animateClipPath);
});
