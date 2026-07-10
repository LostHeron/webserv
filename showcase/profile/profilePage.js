const USERNAME_COOKIE_NAME = "papertrail_username";
const AVATAR_VERSION_COOKIE_NAME = "papertrail_avatar_version";

function setCookie(name, value) {
	const encodedValue = encodeURIComponent(value);
	document.cookie = `${name}=${encodedValue}; path=/`;
}

function readCookie(name) {
	const prefix = `${name}=`;
	const cookie = document.cookie
		.split("; ")
		.find((entry) => entry.startsWith(prefix));

	return cookie ? decodeURIComponent(cookie.slice(prefix.length)) : "";
}

function fileToDataUrl(file) {
	return new Promise((resolve, reject) => {
		const reader = new FileReader();
		reader.onload = () => resolve(reader.result);
		reader.onerror = () => reject(new Error("Unable to read file."));
		reader.readAsDataURL(file);
	});
}

function dataUrlToPngBlob(dataUrl) {
	return new Promise((resolve, reject) => {
		const image = new Image();
		image.onload = () => {
			const canvas = document.createElement("canvas");
			canvas.width = image.width;
			canvas.height = image.height;
			const context = canvas.getContext("2d");

			if (!context) {
				reject(new Error("Canvas unavailable."));
				return;
			}

			context.drawImage(image, 0, 0);
			canvas.toBlob((blob) => {
				if (!blob) {
					reject(new Error("Image conversion failed."));
					return;
				}

				resolve(blob);
			}, "image/png");
		};
		image.onerror = () => reject(new Error("Invalid image."));
		image.src = dataUrl;
	});
}

function sanitizeUsername(name, surname) {
	return `${name}-${surname}`
		.toLowerCase()
		.normalize("NFD")
		.replace(/[\u0300-\u036f]/g, "")
		.replace(/[^a-z0-9]+/g, "-")
		.replace(/^-+|-+$/g, "");
}

function populateForm(form) {
	form.elements.name.value = readCookie("papertrail_name");
	form.elements.surname.value = readCookie("papertrail_surname");
	form.elements.email.value = readCookie("papertrail_email");
}

document.addEventListener("DOMContentLoaded", () => {
	const form = document.getElementById("profile-form");
	if (!form) {
		return;
	}

	populateForm(form);

	form.addEventListener("submit", async (event) => {
		event.preventDefault();

		const name = form.elements.name.value.trim();
		const surname = form.elements.surname.value.trim();
		const email = form.elements.email.value.trim();
		const pictureFile = form.elements["profile-picture"].files[0];

		if (!name || !surname || !email || !pictureFile) {
			alert("Please complete all fields.");
			return;
		}

		setCookie("papertrail_name", name);
		setCookie("papertrail_surname", surname);
		setCookie("papertrail_email", email);

		const username = sanitizeUsername(name, surname);
		if (!username) {
			alert("Please provide a valid name and surname.");
			return;
		}

		setCookie(USERNAME_COOKIE_NAME, username);

		try {
			const originalDataUrl = await fileToDataUrl(pictureFile);
			const pngBlob = await dataUrlToPngBlob(originalDataUrl);
			const avatarPath = `/assets/profile_pictures/${username}.png`;
			const response = await fetch(avatarPath, {
				method: "PUT",
				headers: {
					"Content-Type": "image/png",
				},
				body: pngBlob,
			});

			if (!response.ok) {
				throw new Error("Upload failed.");
			}

			setCookie(AVATAR_VERSION_COOKIE_NAME, String(Date.now()));
		} catch (error) {
			alert("The picture could not be saved.");
			return;
		}

		window.location.href = "/";
	});
});
