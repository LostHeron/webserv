const POSTS_ENDPOINT = "/database/posts.py";

function readCookie(name) {
	const prefix = `${name}=`;
	const cookie = document.cookie
		.split("; ")
		.find((entry) => entry.startsWith(prefix));

	return cookie ? decodeURIComponent(cookie.slice(prefix.length)) : "";
}

async function fetchPosts() {
	const response = await fetch(POSTS_ENDPOINT, { method: "GET", cache: "no-store" });
	if (!response.ok) {
		throw new Error("Unable to load posts.");
	}

	const posts = await response.json();
	return Array.isArray(posts) ? posts : [];
}

function formatDate(value) {
	return new Date(value).toLocaleDateString("en-US", {
		month: "short",
		day: "numeric",
		year: "numeric",
	});
}

function initialsFromName(name) {
	return name
		.split(/\s+/)
		.filter(Boolean)
		.slice(0, 2)
		.map((part) => part[0].toUpperCase())
		.join("") || "?";
}

function avatarMarkup(post) {
	if (!post.avatar) {
		return `<div class="post-avatar">${initialsFromName(post.author || "Anonymous")}</div>`;
	}

	const version = post.avatarVersion ? `?v=${encodeURIComponent(post.avatarVersion)}` : "";
	return `
		<div class="post-avatar image-avatar">
			<img src="/assets/profile_pictures/${post.avatar}.png${version}" alt="${post.author || "Anonymous"} profile picture">
		</div>
	`;
}

function createPostPreview(post) {
	const article = document.createElement("article");
	article.className = "post-item";
	article.innerHTML = `
		<div class="post-preview-header">
			${avatarMarkup(post)}
			<div>
		<span class="pill">${post.author || "Anonymous"}</span>
				<h3>${post.title}</h3>
			</div>
		</div>
		<p>${post.body}</p>
		<p class="muted">${formatDate(post.createdAt)}</p>
	`;
	return article;
}

document.addEventListener("DOMContentLoaded", () => {
	const avatarImage = document.getElementById("profile-avatar-image");
	const postsList = document.getElementById("home-posts-list");
	const emptyState = document.getElementById("home-empty-state");

	if (postsList && emptyState) {
		(async () => {
			try {
				const posts = (await fetchPosts())
					.slice()
					.sort((left, right) => new Date(right.createdAt) - new Date(left.createdAt));
				const recentPosts = posts.slice(0, 5);

				postsList.innerHTML = "";
				emptyState.hidden = recentPosts.length !== 0;

				recentPosts.forEach((post) => {
					postsList.appendChild(createPostPreview(post));
				});
			} catch {
				postsList.innerHTML = "";
				emptyState.hidden = false;
			}
		})();
	}

	if (!avatarImage) {
		return;
	}

	const username = readCookie("papertrail_username");
	if (username) {
		const version = readCookie("papertrail_avatar_version");
		const cacheBuster = version ? `?v=${encodeURIComponent(version)}` : "";
		avatarImage.src = `/assets/profile_pictures/${username}.png${cacheBuster}`;
	}
});
