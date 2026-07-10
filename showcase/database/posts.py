#!/usr/bin/env python3

import json
import os
import sys
from pathlib import Path
from datetime import datetime
from urllib.parse import parse_qs
from http.cookies import SimpleCookie
from html import escape


DATA_FILE = Path(__file__).with_name("posts.json")


def load_posts():
	if not DATA_FILE.exists():
		return []

	try:
		with DATA_FILE.open("r", encoding="utf-8") as file_handle:
			posts = json.load(file_handle)
		return posts if isinstance(posts, list) else []
	except Exception:
		return []


def save_posts(posts):
	with DATA_FILE.open("w", encoding="utf-8") as file_handle:
		json.dump(posts, file_handle, indent=2)
		file_handle.write("\n")


def read_body():
	try:
		content_length = int(os.environ.get("CONTENT_LENGTH", "0"))
	except ValueError:
		content_length = 0
	return sys.stdin.read(content_length) if content_length > 0 else ""


def read_json_body():
	body = read_body().strip()
	if not body:
		return {}
	try:
		return json.loads(body)
	except json.JSONDecodeError:
		parsed = parse_qs(body, keep_blank_values=True)
		return {key: values[0] if values else "" for key, values in parsed.items()}


def content_type_is_json():
	return "application/json" in os.environ.get("CONTENT_TYPE", "")


def get_query_value(name):
	query_string = os.environ.get("QUERY_STRING", "")
	for chunk in query_string.split("&"):
		if not chunk:
			continue
		key, _, value = chunk.partition("=")
		if key == name:
			return value
	return ""


def get_cookie_value(name):
	cookie_header = os.environ.get("HTTP_COOKIE", "")
	if not cookie_header:
		return ""

	cookie = SimpleCookie()
	cookie.load(cookie_header)
	if name not in cookie:
		return ""
	return cookie[name].value


def get_request_author(post_data):
	author = post_data.get("author", "").strip()
	if author:
		return author

	name = get_cookie_value("papertrail_name")
	surname = get_cookie_value("papertrail_surname")
	combined = f"{name} {surname}".strip()
	return combined or "Anonymous"


def get_request_avatar(post_data):
	return post_data.get("avatar", "").strip()


def get_request_avatar_version(post_data):
	return post_data.get("avatarVersion", "").strip()


def json_response(status_code, payload):
	print(f"Status: {status_code}")
	print("Content-Type: application/json")
	print()
	print(json.dumps(payload))


def html_response(status_code, title, message, back_link="/blog"):
	print(f"Status: {status_code}")
	print("Content-Type: text/html; charset=utf-8")
	print()
	print(f"""<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>{escape(title)}</title>
	<style>
		body {{
			font-family: system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
			margin: 0;
			min-height: 100vh;
			display: grid;
			place-items: center;
			background: #f6f1ea;
			color: #1f2430;
		}}
		.card {{
			background: white;
			padding: 2rem 2.25rem;
			border-radius: 20px;
			box-shadow: 0 12px 30px rgba(31, 36, 48, 0.12);
			text-align: center;
			max-width: 520px;
		}}
		a {{
			display: inline-block;
			margin-top: 1rem;
			padding: 0.8rem 1.1rem;
			border-radius: 999px;
			background: #7f6cff;
			color: white;
			text-decoration: none;
			font-weight: 700;
		}}
	</style>
</head>
<body>
	<main class="card">
		<h1>{escape(title)}</h1>
		<p>{escape(message)}</p>
		<a href="{escape(back_link)}">Back to blog</a>
	</main>
</body>
</html>""")


def create_post(post_data, posts):
	post = {
		"id": post_data.get("id") or os.urandom(8).hex(),
		"author": get_request_author(post_data),
		"avatar": get_request_avatar(post_data),
		"avatarVersion": get_request_avatar_version(post_data),
		"title": post_data.get("title", ""),
		"body": post_data.get("body", ""),
		"likes": int(post_data.get("likes", 0)),
		"createdAt": post_data.get("createdAt") or datetime.utcnow().isoformat(),
	}
	posts.insert(0, post)
	return posts


def update_post(post_data, posts):
	post_id = post_data.get("id", "")
	updated_posts = []
	for post in posts:
		if post.get("id") == post_id:
			updated_posts.append({
				**post,
				"author": get_request_author(post_data) if post_data.get("author") else post.get("author", "Anonymous"),
				"avatar": get_request_avatar(post_data) if post_data.get("avatar") else post.get("avatar", ""),
				"avatarVersion": get_request_avatar_version(post_data) if post_data.get("avatarVersion") else post.get("avatarVersion", ""),
				"title": post_data.get("title", post.get("title", "")),
				"body": post_data.get("body", post.get("body", "")),
				"likes": int(post_data.get("likes", post.get("likes", 0))),
				"updatedAt": datetime.utcnow().isoformat(),
			})
		else:
			updated_posts.append(post)
	return updated_posts


def like_post(post_data, posts):
	post_id = post_data.get("id", "")
	liked_posts = []
	for post in posts:
		if post.get("id") == post_id:
			liked_posts.append({**post, "likes": int(post.get("likes", 0)) + 1})
		else:
			liked_posts.append(post)
	return liked_posts


def delete_post(post_id, posts):
	return [post for post in posts if post.get("id") != post_id]


def main():
	method = os.environ.get("REQUEST_METHOD", "GET").upper()
	posts = load_posts()

	if method == "GET":
		json_response(200, posts)
		return

	if method == "POST":
		payload = read_json_body()
		if not content_type_is_json():
			post_data = payload
			posts = create_post(post_data, posts)
			save_posts(posts)
			html_response(
				200,
				"Post published",
				"Your post was published successfully.",
				"/blog",
			)
			return

		action = payload.get("action", "create")
		post_data = payload.get("post", payload)

		if action == "create":
			posts = create_post(post_data, posts)
		elif action == "update":
			posts = update_post(post_data, posts)
		elif action == "like":
			posts = like_post(post_data, posts)
		else:
			json_response(400, {"error": "Unsupported action."})
			return

		save_posts(posts)
		json_response(200, posts)
		return

	if method == "DELETE":
		payload = read_json_body()
		post_id = payload.get("id") or get_query_value("id")
		if not post_id:
			json_response(400, {"error": "Missing post id."})
			return

		posts = delete_post(post_id, posts)
		save_posts(posts)
		json_response(200, posts)
		return

	json_response(405, {"error": "Method not allowed."})


if __name__ == "__main__":
	main()