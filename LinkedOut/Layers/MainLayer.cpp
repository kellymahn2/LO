#include "MainLayer.h"
#include "Core/Window.h"
#include "Core/Application.h"
#include "Core/Random.h"
#include "Core/PasswordUtils.h"
#include "CustomUI/PopupWindow.h"


#include <QMainWindow>
#include <qpushbutton.h>
#include "ui_LinkedOut.h"
#include <QStyle>
#include <QStandardPaths>
#include <qlabel.h>
#include <qlayout.h>
#include <QValidator>
#include <qvalidator.h>
#include <QTreeWidget>
#include <QDate>
#include <QBoxLayout>
#include <QCryptographicHash>
#include <iostream>

namespace LinkedOut {

	static constexpr const char* TempFileName = "LinkedOutUD.txt";

	static QIcon LoadIcon(const std::string& icon) {
		return QIcon(QString::fromStdString(icon));
	}


	UserInternalData MainLayer::GetUserDataStoredLocally() {
		FileSystem::path tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation).toStdString();

		FileSystem::path tempFilePath = tempPath / TempFileName;
		if (FileSystem::directory_entry(tempPath).exists()) {

			std::ifstream tempFile(tempFilePath);

			if (tempFile.is_open() == false)
				return {};

			std::string username;
			std::string password;

			{
				char buff[10000];
				tempFile.getline(buff, 10000);
				username = buff;
			}

			{
				char buff[10000];
				tempFile.getline(buff, 10000);
				password = buff;
			}

			TrimInplace(username);
			TrimInplace(password);

			if (!IsValidUsername(username))
				return {};
			if (!IsValidPassword(password))
				return {};



			return { std::move(username), std::move(password) };
		}
		return {};
	}

	void MainLayer::StoreUserDataLocally(const UserInternalData& userData) {
		FileSystem::path tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation).toStdString();
		FileSystem::path tempFilePath = tempPath / TempFileName;
		std::ofstream file(tempFilePath);
		if (!file.is_open())
			return;
		file.clear();
		file << userData.Username;
		file << "\n";
		file << userData.Password;
		file.close();
		return;
	}

	Ui_LinkedOutClass* cl;
	MainLayer* MainLayer::s_Instance = nullptr;


	/*void MainLayer::SwitchToLogin(bool cleanAll) {
		SetCurrentLayer(m_LoginLayer);
		m_LastCaptchaNumber = Random::GenerateUInt32(1000, 10000);
		m_LoginLayer->m_CaptchaLabel->setText(QString("Captcha %1").arg(m_LastCaptchaNumber));
		if (cleanAll)
			m_LoginLayer->CleanAllInputs();
		else
			m_LoginLayer->CleanupInputs();
	}

	void MainLayer::SwitchToUserInformation()
	{
		if (m_CurrentLayer == m_LoginLayer) {
			UserData data = GetUserInformation(m_UserData.UniqueUserID);
			m_UserInformationLayer->SetContext(data);
		}
		
		SetCurrentLayer(m_UserInformationLayer);
	}*/

	//void MainLayer::SwitchToHome()
	//{
	//	//SetCurrentLayer(m_HomeLayer);
	//}

	Ref<Person> MainLayer::GetPerson(const std::string& id)
	{
		if (m_IDToPersonMap.find(id) != m_IDToPersonMap.end())
			return m_IDToPersonMap.at(id);
		else {
			Ref<Person> person = CreateRef<Person>();
			m_IDToPersonMap[id] = person;
			QueryPerson(id, person);

			return person;
		}
		return {};
	}

	Ref<Company> MainLayer::GetCompany(const std::string& id)
	{
		if (m_IDToCompanyMap.find(id) != m_IDToCompanyMap.end())
			return m_IDToCompanyMap.at(id);
		else {
			Ref<Company> company = CreateRef<Company>();
			m_IDToCompanyMap[id] = company;
			QueryCompany(id, company);

			return company;
		}
		return {};
	}

	Ref<Job> MainLayer::GetJob(const std::string& id)
	{
		if (m_IDToJobMap.find(id) != m_IDToJobMap.end())
			return m_IDToJobMap.at(id);
		else {
			Ref<Job> job = CreateRef<Job>();
			m_IDToJobMap[id] = job;
			QueryJob(id, job);

			return job;
		}
		return {};
	}

	Ref<Post> MainLayer::GetPost(const std::string& id)
	{
		if (m_IDToPostMap.find(id) != m_IDToPostMap.end())
			return m_IDToPostMap.at(id);
		else {
			Ref<Post> post= CreateRef<Post>();
			m_IDToPostMap[id] = post;
			QueryPost(id, post);

			return post;
		}
		return {};
	}

	/*Ref<DirectMessage> MainLayer::GetDirectMessage(const std::string& id)
	{
		if (m_IDToDM.find(id) != m_IDToDM.end())
			return m_IDToDM.at(id);
		else {
			Ref<DirectMessage> dm = CreateRef<DirectMessage>();
			m_IDToDM[id] = dm;
			QueryDirectMessage(id, dm);

			return dm;
		}
		return {};
	}*/

	Ref<Person> MainLayer::QueryPerson(const std::string& id,Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();

		person->SetAccountID(id);

		std::string queryPerson = fmt::format("SELECT * FROM Account a JOIN Person p ON a.account_id = p.account_id WHERE a.account_id = {}", id);
		auto results = db->ExecuteAndReturn(queryPerson);
		assert(results.size() == 1);

		auto& personDet = results[0];

		std::string phone_number = personDet.value("phone_number").toString().toStdString();
		person->SetPhoneNumber(phone_number);
		std::string user_name = personDet.value("user_name").toString().toStdString();
		person->SetUsername(user_name);
		std::string password = personDet.value("password").toString().toStdString();
		person->SetPassword(password);
		std::string email = personDet.value("email").toString().toStdString();
		person->SetEmail(email);
		std::string image_path = personDet.value("image_path").toString().toStdString();
		person->SetImagePath(image_path);
		std::string first_name = personDet.value("first_name").toString().toStdString();
		person->SetFirstName(first_name);
		std::string last_name = personDet.value("last_name").toString().toStdString();
		person->SetLastName(last_name);

		{
			std::string querySkills = fmt::format("SELECT * FROM PersonSkills WHERE (person_id = {})", id);
			auto skills = db->ExecuteAndReturn(querySkills);
			std::vector<std::string> personSkills;
			for (auto& skill : skills) {
				personSkills.push_back(skill.value("skill_tag").toString().toStdString());
			}
			person->SetSkills(personSkills);
		}

		{
			std::string queryFollowing = fmt::format("SELECT * FROM Following WHERE (follower_id = {});", id);
			auto followings = db->ExecuteAndReturn(queryFollowing);

			std::vector<std::string> followingIDs;
			for (auto& id : followings) {
				followingIDs.push_back(id.value("following_id").toString().toStdString());
			}
			person->SetFollowingID(followingIDs);
		}

		{
			std::string queryPosts = fmt::format(R"(
			SELECT 
			    p.post_id,
			    p.time_sent
			FROM 
			    Post p
			WHERE
			    p.sender_id = {0}

			UNION ALL

			SELECT 
			    r.post_id,
			    r.repost_at AS time_sent
			FROM 
			    Repost r
			JOIN 
			    Post p ON r.post_id = p.post_id
			WHERE
			    r.account_id = {0}

			ORDER BY 
			    time_sent DESC;
			)", id);
			auto posts = db->ExecuteAndReturn(queryPosts);

			std::vector<Ref<Post>> userPosts;

			for (auto& post : posts) {
				userPosts.push_back(GetPost(post.value("post_id").toString().toStdString()));
			}
			person->SetPosts(userPosts);
		}

		{
			std::string queryDMS = fmt::format(R"(SELECT * FROM DirectMessage WHERE sender_id = {} OR receiver_id = {} ORDER BY time_sent; )", id,id);
			auto dms = db->ExecuteAndReturn(queryDMS);

			//With -> dm
			std::unordered_map<std::string, std::vector<Ref<DirectMessage>>> userDMs;

			for (auto& dm : dms) {

				Ref<DirectMessage> message = CreateRef<DirectMessage>();

				std::string messageID = dm.value("message_id").toString().toStdString();
				std::string toWho = dm.value("receiver_id").toString().toStdString();
				std::string senderID = dm.value("sender_id").toString().toStdString();
				std::string timeSent = dm.value("time_sent").toString().toStdString();
				std::string contentText = dm.value("content_text").toString().toStdString();
				std::string contentPicture = dm.value("content_picture").toString().toStdString();
				std::string contentVideo = dm.value("content_video").toString().toStdString();

				message->SetMessageID(messageID);
				message->SetToWho(toWho);
				message->SetSenderID(senderID);
				message->SetContentText(contentText);
				message->SetContentPicture(contentPicture);
				message->SetContentVideo(contentVideo);

				Time timeObj;
				timeObj.ExtractFromTimestamp(timeSent);
				message->SetTimeSent(timeObj);

				if (senderID == id) {
					userDMs[toWho].push_back(message);
				}
				else if (toWho == id) {
					userDMs[senderID].push_back(message);
				}

			}

			person->SetDirectMessages(userDMs);
		}

		{
			std::string queryJob = fmt::format("SELECT * FROM CompanyEmployees WHERE (employee_id = {});", id);
			auto result = db->ExecuteAndReturn(queryJob);
			assert(result.size() < 2);
			if (!result.empty()) {
				person->SetJob(GetJob(result[0].value("job_id").toString().toStdString()));
			}
		}

		return person;
	}

	Ref<Company> MainLayer::QueryCompany(const std::string& id,Ref<Company> company)
	{
		auto db = Application::Get()->GetDatabase();
		company->SetAccountID(id);
		std::string queryCompany = fmt::format("SELECT * FROM Account a JOIN Company c ON a.account_id = c.account_id WHERE a.account_id = {}", id);

		auto results = db->ExecuteAndReturn(queryCompany);
		assert(results.size() == 1);

		auto& companyDet = results[0];

		std::string phone_number = companyDet.value("phone_number").toString().toStdString();
		company->SetPhoneNumber(phone_number);
		std::string user_name = companyDet.value("user_name").toString().toStdString();
		company->SetUsername(user_name);
		std::string password = companyDet.value("password").toString().toStdString();
		company->SetPassword(password);
		std::string email = companyDet.value("email").toString().toStdString();
		company->SetEmail(email);
		std::string image_path = companyDet.value("image_path").toString().toStdString();
		company->SetImagePath(image_path);
		std::string company_name = companyDet.value("company_name").toString().toStdString();
		company->SetCompanyName(company_name);

		std::string queryJobs = fmt::format("SELECT job_id FROM CompanyJobs WHERE (company_id = {} AND taken = 0);", id);
		auto jobs = db->ExecuteAndReturn(queryJobs);

		std::vector<Ref<Job>> companyJobs;

		for (auto& job : jobs) {
			companyJobs.push_back(GetJob(job.value("job_id").toString().toStdString()));
		}
		company->SetPostedJobs(companyJobs);

		std::string queryEmployees = fmt::format("SELECT employee_id FROM CompanyEmployees WHERE (company_id = {});", id);
		auto employees = db->ExecuteAndReturn(queryEmployees);

		std::vector<Ref<Person>> companyEmployees;

		for (auto& employee : employees) {
			companyEmployees.push_back(GetPerson(employee.value("employee_id").toString().toStdString()));
		}
		company->SetEmployees(companyEmployees);

		{
			std::string queryFollowing = fmt::format("SELECT * FROM Following WHERE (follower_id = {});", id);
			auto followings = db->ExecuteAndReturn(queryFollowing);

			std::vector<std::string> followingIDs;
			for (auto& id : followings) {
				followingIDs.push_back(id.value("following_id").toString().toStdString());
			}
			company->SetFollowingID(followingIDs);
		}

		{
			std::string queryPosts = fmt::format(R"(
			SELECT 
			    p.post_id,
			    p.time_sent
			FROM 
			    Post p
			WHERE
			    p.sender_id = {0}

			UNION ALL

			SELECT 
			    r.post_id,
			    r.repost_at AS time_sent
			FROM 
			    Repost r
			JOIN 
			    Post p ON r.post_id = p.post_id
			WHERE
			    r.account_id = {0}

			ORDER BY 
			    time_sent DESC;
			)", id);
			auto posts = db->ExecuteAndReturn(queryPosts);

			std::vector<Ref<Post>> userPosts;

			for (auto& post : posts) {
				userPosts.push_back(GetPost(post.value("post_id").toString().toStdString()));
			}
			company->SetPosts(userPosts);
		}

		return company;
	}

	Ref<Job> MainLayer::QueryJob(const std::string& id,Ref<Job> job)
	{
		auto db = Application::Get()->GetDatabase();
		
		std::string queryJob = fmt::format("SELECT * FROM CompanyJobs WHERE (job_id = {});", id);
		auto _rows = db->ExecuteAndReturn(queryJob);

		assert(_rows.size() == 1);
		auto& row = _rows[0];

		job->SetID(id);
		int salary = row.value("salary").toInt(); 
		job->SetSalary(salary);
		std::string job_name = row.value("job_name").toString().toStdString(); 
		job->SetJobName(job_name);
		std::string company_name = row.value("company_name").toString().toStdString(); 
		job->SetCompanyName(company_name);
		WorkspaceType workspace_type = (WorkspaceType)row.value("workspace_type").toInt();
		job->SetWorkspaceType(workspace_type);
		std::string location = row.value("location").toString().toStdString(); 
		job->SetLocation(location);
		JobType job_type = (JobType)row.value("job_type").toInt();
		job->SetJobType(job_type);

		std::string querySkills = fmt::format("SELECT * FROM JobSkills WHERE (job_id = {});", id);
		auto skills = db->ExecuteAndReturn(querySkills);

		std::vector<std::string> requiredSkills;

		for (auto& skill : skills) {
			requiredSkills.push_back(skill.value("skill_tag").toString().toStdString());
		}
		job->SetRequiredSkills(requiredSkills);
		return job;
	}

	Ref<Post> MainLayer::QueryPost(const std::string& id, Ref<Post> post)
	{
		auto db = Application::Get()->GetDatabase();

		post->SetPostID(id);

		std::string queryPost = fmt::format(R"(
			SELECT * FROM Post WHERE (post_id = {});
		)",id);

		auto results = db->ExecuteAndReturn(queryPost);

		assert(results.size() == 1);
		auto& postDet = results[0];

		std::string poster_name = postDet.value("poster_name").toString().toStdString();
		post->SetPosterName(poster_name);
		std::string sender_id = postDet.value("sender_id").toString().toStdString();
		post->SetSenderID(sender_id);
		std::string time_sent = postDet.value("time_sent").toString().toStdString();
		{
			Time t;
			t.ExtractFromTimestamp(time_sent);
			post->SetTimeSent(t);
		}
		std::string content_text = postDet.value("content_text").toString().toStdString();
		post->SetContentText(content_text);
		std::string content_picture = postDet.value("content_picture").toString().toStdString();
		post->SetContentPicture(content_picture);
		std::string content_video = postDet.value("content_video").toString().toStdString();
		post->SetContentVideo(content_video);
		int repost_counter = 0;
		{
			std::string queryReposts = fmt::format("SELECT * FROM Repost WHERE (post_id = {});", id);
			auto results = db->ExecuteAndReturn(queryReposts);
			repost_counter = results.size();
		}
		post->SetRepostCounter(repost_counter);
		

		std::string queryLikes = fmt::format("SELECT * FROM Like WHERE (post_id = {}) ORDER BY liked_at", id);
		auto likes = db->ExecuteAndReturn(queryLikes);

		std::vector<Like> postLikes;

		for (auto& like : likes) {
			Like l;
			l.SetLikedBy(like.value("liked_by").toString().toStdString());
			l.SetLikeID(like.value("like_id").toString().toStdString());
			Time t;
			t.ExtractFromTimestamp(like.value("liked_at").toString().toStdString());
			l.SetLikedAt(t);
			postLikes.push_back(l);
		}

		post->SetLikes(postLikes);

		std::string queryComments = fmt::format("SELECT * FROM Comment WHERE (post_id = {});", id);
		auto comments = db->ExecuteAndReturn(queryComments);

		std::vector<Ref<Comment>> postComments;

		for (auto& comment : comments) {
			Ref<Comment> c = CreateRef<Comment>();
			c->SetCommentID(comment.value("comment_id").toString().toStdString());
			c->SetPostID(id);
			c->SetSenderID(comment.value("sender_id").toString().toStdString());
			Time t;
			t.ExtractFromTimestamp(comment.value("time_sent").toString().toStdString());
			c->SetTimeSent(t);
			c->SetContentText(comment.value("content_text").toString().toStdString());
			c->SetContentPicture(comment.value("content_picture").toString().toStdString());
			c->SetContentVideo(comment.value("content_video").toString().toStdString());
			c->SetCommenterName(comment.value("commenter_name").toString().toStdString());
			postComments.push_back(c);
		}
		post->SetComments(postComments);
		return post;
	}

	Ref<DirectMessage> MainLayer::QueryDirectMessage(const std::string& id, Ref<DirectMessage> message)
	{
		auto db = Application::Get()->GetDatabase();

		return message;
	}

	bool MainLayer::AccountExists(const std::string& username)
	{
		auto db = Application::Get()->GetDatabase();

		std::string query = fmt::format("SELECT * FROM Account WHERE (user_name = '{}');", username);
		auto result = db->ExecuteAndReturn(query);
		assert(result.size() < 2);

		return !result.empty();
	}

	bool MainLayer::AccountExists(const std::string& username, const std::string& password)
	{
		auto db = Application::Get()->GetDatabase();

		std::string query = fmt::format("SELECT * FROM Account WHERE (user_name = '{}' AND password = '{}');", username,password);
		auto result = db->ExecuteAndReturn(query);
		assert(result.size() < 2);

		return !result.empty();
	}

	bool MainLayer::PostExists(const std::string& id)
	{
		auto db = Application::Get()->GetDatabase();

		std::string query = fmt::format("SELECT * FROM POST WHERE post_id = {};", id);

		auto res = db->ExecuteAndReturn(query);

		return !res.empty();
	}

	std::string MainLayer::CompanyAccountExists(const std::string& username)
	{
		auto db = Application::Get()->GetDatabase();

		std::string query = fmt::format(R"(SELECT * FROM Company c
											JOIN Account a ON c.account_id = a.account_id 
											WHERE (a.user_name = '{}');)",username);
		auto result = db->ExecuteAndReturn(query);
		assert(result.size() < 2);

		if (result.empty())
			return "";
		return result[0].value("account_id").toString().toStdString();
	}

	std::string MainLayer::CompanyAccountExists(const std::string& username, const std::string& password)
	{
		auto db = Application::Get()->GetDatabase();

		std::string query = fmt::format(R"(SELECT * FROM Company c
											JOIN Account a ON c.account_id = a.account_id 
											WHERE (a.user_name = '{}' AND a.password = '{}');)", username,password);
		auto result = db->ExecuteAndReturn(query);
		assert(result.size() < 2);

		if (result.empty())
			return "";
		return result[0].value("account_id").toString().toStdString();
	}

	std::string MainLayer::PersonAccountExists(const std::string& username)
	{
		auto db = Application::Get()->GetDatabase();

		std::string query = fmt::format(R"(SELECT * FROM Person
											JOIN Account ON Person.account_id = Account.account_id 
											WHERE (Account.user_name = '{}');)", username);
		auto result = db->ExecuteAndReturn(query);
		assert(result.size() < 2);

		if (result.empty())
			return "";
		return result[0].value("account_id").toString().toStdString();
	}

	std::string MainLayer::PersonAccountExists(const std::string& username, const std::string& password)
	{
		auto db = Application::Get()->GetDatabase();

		std::string query = fmt::format(R"(SELECT * FROM Person 
											JOIN Account ON Person.account_id = Account.account_id 
											WHERE (Account.user_name = '{}' AND Account.password = '{}');)", username, password);
		auto result = db->ExecuteAndReturn(query);
		assert(result.size() < 2);

		if (result.empty())
			return "";
		return result[0].value("account_id").toString().toStdString();
	}

	void MainLayer::LikePost(Ref<Post> post)
	{
		if (std::find_if(post->GetLikes().begin(), post->GetLikes().end(), [this,post](const Like& like) {
			return like.GetLikedBy() == m_CurrentUser->GetAccountID();
			}) != post->GetLikes().end()) {
			return;
		}
		auto db = Application::Get()->GetDatabase();

		{
			std::string cmd = fmt::format("INSERT INTO Like(liked_by,post_id) VALUES({},{});",m_CurrentUser->GetAccountID(),post->GetPostID());
			if (!db->Execute(cmd))
				return;
			db->Commit();
		}
		Like like;

		{
			std::string cmd = fmt::format("SELECT * FROM Like WHERE (liked_by = {} AND post_id = {});",m_CurrentUser->GetAccountID(),post->GetPostID());
			auto res = db->ExecuteAndReturn(cmd);
			assert(res.size() == 1);

			auto& record = res[0];

			std::string like_id = std::to_string(record.value("like_id").toInt());
			std::string liked_at = record.value("liked_at").toString().toStdString();
			
			Time time;
			time.ExtractFromTimestamp(liked_at);

			like.SetLikeID(like_id);
			like.SetLikedAt(time);
			like.SetLikedBy(m_CurrentUser->GetAccountID());
		}

		post->LikePost(like);
	}

	void MainLayer::RepostPost(Ref<Post> post)
	{
		auto db = Application::Get()->GetDatabase();
		{
			std::string cmd = fmt::format("INSERT INTO Repost(post_id,account_id) VALUES({},{});", post->GetPostID(), m_CurrentUser->GetAccountID());
			db->Execute(cmd);
		}
	}

	void MainLayer::CommentOnPost(Ref<Post> post,const std::string& text,const std::string& picture,const std::string& video)
	{
		auto db = Application::Get()->GetDatabase();
		{
			std::string cmd = fmt::format("INSERT INTO Comment (post_id,sender_id,content_text,content_picture,content_video,commenter_name) VALUES({},{},'{}','{}','{}','{}');",
											post->GetPostID(), m_CurrentUser->GetAccountID(), text,
											picture, video, m_CurrentUser->GetUsername());
			if (!db->Execute(cmd))
				return;
			db->Commit();
		}

		Ref<Comment> c = CreateRef<Comment>();

		{
			std::string cmd = fmt::format("SELECT comment_id,time_sent FROM Comment WHERE comment_id = last_insert_rowid()");
			auto res = db->ExecuteAndReturn(cmd);
			assert(res.size() == 1);
			auto& rec = res[0];

			std::string comment_id = rec.value("comment_id").toString().toStdString();
			std::string time_sent = rec.value("time_sent").toString().toStdString();
			c->SetCommenterName(m_CurrentUser->GetUsername());
			c->SetCommentID(comment_id);
			Time t;
			t.ExtractFromTimestamp(time_sent);
			c->SetTimeSent(t);
			c->SetContentText(text);
			c->SetContentPicture(picture);
			c->SetContentVideo(video);
			c->SetPostID(post->GetPostID());
			c->SetSenderID(m_CurrentUser->GetAccountID());
		}
		post->CommentOnPost(c);
	}

	void MainLayer::Follow(const std::string& followerID, const std::string& followingID)
	{
		if (followerID == followingID)
			return;
		auto db = Application::Get()->GetDatabase();
		{
			std::string cmd = fmt::format("SELECT * FROM Following WHERE (follower_id = {} AND following_id = {});", followerID, followingID);
			auto result = db->ExecuteAndReturn(cmd);
			assert(result.size() < 2);
			if (!result.empty())
				return;
		}
		
		
		if (auto ptr = m_IDToPersonMap.find(followerID); ptr != m_IDToPersonMap.end()) {
			ptr->second->Follow(followingID);
		}

		std::string insertFollowing = fmt::format("INSERT INTO Following(follower_id,following_id) VALUES ({},{})", followerID,followingID);
		db->Execute(insertFollowing);
	}

	void MainLayer::RequestFollow(const std::string& id)
	{
		if (m_CurrentUser->GetAccountID() == id)
			return;

		auto db = Application::Get()->GetDatabase();
		{
			std::string cmd = fmt::format("SELECT * FROM FollowRequest WHERE (requester_id = {} AND person_id = {});", m_CurrentUser->GetAccountID(), id);
			auto result = db->ExecuteAndReturn(cmd);
			assert(result.size() < 2);
			if (!result.empty())
				return;
		}
		std::string insertFollowing = fmt::format("INSERT INTO FollowRequest(requester_id,person_id) VALUES ({},{})", m_CurrentUser->GetAccountID(), id);
		db->Execute(insertFollowing);
	}

	void MainLayer::MakePost(const std::string& text, const std::string& picture, const std::string& video)
	{
		Ref<Post> post = CreateRef<Post>();

		post->SetContentText(text);
		post->SetContentPicture(picture);
		post->SetContentVideo(video);
		post->SetPosterName(m_CurrentUser->GetUsername());
		post->SetSenderID(m_CurrentUser->GetAccountID());

		auto db = Application::Get()->GetDatabase();
		std::string insertPost = fmt::format("INSERT INTO Post(sender_id,poster_name,content_text,content_picture,content_video) VALUES({},'{}','{}','{}','{}'); ", 
									m_CurrentUser->GetAccountID(),m_CurrentUser->GetUsername(), text, picture, video);
		db->Execute(insertPost);

		{
			std::string cmd = fmt::format("SELECT post_id,time_sent FROM Post WHERE post_id = last_insert_rowid()");
			auto res = db->ExecuteAndReturn(cmd);
			assert(res.size() == 1);
			auto& rec = res[0];

			std::string post_id = rec.value("post_id").toString().toStdString();
			std::string time_sent = rec.value("time_sent").toString().toStdString();
			Time t;
			t.ExtractFromTimestamp(time_sent);
			post->SetTimeSent(t);
			post->SetPostID(post_id);
			m_IDToPostMap[post_id] = post;
		}
		m_CurrentUser->Post(post);
	}

	Ref<DirectMessage> MainLayer::SendMessage(Ref<Person> receiver, const std::string& text, const std::string& picture, const std::string& video)
	{
		auto db = Application::Get()->GetDatabase();

		std::string insertMessage = fmt::format("INSERT INTO DirectMessage(sender_id, content_text, content_picture, content_video, receiver_id) VALUES({},'{}','{}','{}',{});"
													,m_CurrentUser->GetAccountID(),text,picture,video,receiver->GetAccountID() );
		db->Execute(insertMessage);

		Ref<DirectMessage> message = CreateRef<DirectMessage>();

		{
			std::string queryMessage = fmt::format("SELECT message_id,time_sent FROM DirectMessage WHERE message_id = last_insert_rowid()");
			auto results = db->ExecuteAndReturn(queryMessage);

			assert(results.size() == 1);

			auto& record = results[0];

			std::string message_id = record.value("message_id").toString().toStdString();
			message->SetMessageID(message_id);
			std::string time_sent = record.value("time_sent").toString().toStdString();
			Time t;
			t.ExtractFromTimestamp(time_sent);
			message->SetTimeSent(t);
			message->SetSenderID(m_CurrentUser->GetAccountID());
			message->SetToWho(receiver->GetAccountID());
			message->SetContentText(text);
			message->SetContentPicture(picture);
			message->SetContentVideo(video);
		}
		m_CurrentUser->DM(message, receiver->GetAccountID());
		receiver->DM(message, m_CurrentUser->GetAccountID());
		return message;
	}

	Ref<Job> MainLayer::CreateJob(Ref<Company> company, int32_t salary, const std::string& name, const std::string& location, JobType jobType, WorkspaceType workspaceType, const std::vector<std::string>& skills)
	{
		auto db = Application::Get()->GetDatabase();
		
		Ref<Job> job = company->CreateJob();

		std::string insertJob = fmt::format("INSERT INTO CompanyJobs(company_id,salary,job_name,company_name,workspace_type,location,job_type) VALUES({},{},'{}','{}',{},'{}',{});", 
												company->GetAccountID(), salary, name, company->GetCompanyName(), (int)workspaceType, location, (int)jobType);
		db->Execute(insertJob);

		{
			std::string queryJob = fmt::format("SELECT job_id FROM CompanyJobs WHERE job_id = last_insert_rowid();");
			auto results = db->ExecuteAndReturn(queryJob);
			assert(results.size() == 1);
			std::string job_id = results[0].value("job_id").toString().toStdString();

			std::string insertSkill = "INSERT INTO JobSkills VALUES";

			for (auto& skill : skills) {
				insertSkill += fmt::format("({},'{}'),", job_id, skill);
			}
			if (skills.size() != 0) {
				insertSkill.pop_back();
				insertSkill.push_back(';');
				db->Execute(insertSkill);
			}

			job->SetJobName(name);
			job->SetCompanyName(company->GetCompanyName());
			job->SetSalary(salary);
			job->SetWorkspaceType(workspaceType);
			job->SetLocation(location);
			job->SetJobType(jobType);
			job->SetRequiredSkills(skills);
		}

		return job;
	}

	void MainLayer::SignupCompany(const std::string& username, const std::string& password)
	{
		auto db = Application::Get()->GetDatabase();

		std::string insertCompany = fmt::format("INSERT INTO Account(user_name,password) VALUES ('{}','{}');", username, password);
		db->Execute(insertCompany);
		std::string id;

		{
			std::string queryID = fmt::format("SELECT account_id FROM Account WHERE (account_id = last_insert_rowid());");
			auto result = db->ExecuteAndReturn(queryID);
			assert(result.size() == 1);
			id = result[0].value("account_id").toString().toStdString();

			std::string insert = fmt::format("INSERT INTO Company(account_id) VALUES({})", id);
			db->Execute(insert);
		}

		LoginCompany(id);
	}

	void MainLayer::SignupPerson(const std::string& username, const std::string& password)
	{
		auto db = Application::Get()->GetDatabase();

		std::string insertPerson = fmt::format("INSERT INTO Account(user_name,password) VALUES ('{}','{}');", username, password);
		db->Execute(insertPerson);
		std::string id;

		{
			std::string queryID = fmt::format("SELECT account_id FROM Account WHERE (account_id = last_insert_rowid());");
			auto result = db->ExecuteAndReturn(queryID);
			assert(result.size() == 1);
			id = result[0].value("account_id").toString().toStdString();

			std::string insert = fmt::format("INSERT INTO Person(account_id) VALUES({})", id);
			db->Execute(insert);
		}

		LoginPerson(id);
	}

	void MainLayer::LoginCompany(const std::string& id)
	{
		m_CurrentUser = GetCompany(id);
	}

	void MainLayer::LoginPerson(const std::string& id)
	{
		m_CurrentUser = GetPerson(id);
	}

	void MainLayer::AcceptFollow(Ref<Person> person)
	{
		Follow(person->GetAccountID(), m_CurrentUser->GetAccountID());
		auto db = Application::Get()->GetDatabase();

		std::string cmd = fmt::format("DELETE FROM FollowRequest WHERE requester_id = {} AND person_id = {}", person->GetAccountID(), m_CurrentUser->GetAccountID());
		db->Execute(cmd);
	}

	void MainLayer::MakeJobRequest(const std::string& id)
	{
		auto db = Application::Get()->GetDatabase();
		std::string insertRequest = fmt::format("INSERT INTO CompanyJobRequest(job_id,requester_id) VALUES({},{});", id, m_CurrentUser->GetAccountID());
		db->Execute(insertRequest);
	}

	void MainLayer::AcceptJobRequest(Ref<Company> company,Ref<Job> job, Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();
		std::string removeRequest = fmt::format("DELETE FROM CompanyJobRequest WHERE (job_id = {});",job->GetID());
		std::string removeJob = fmt::format("UPDATE CompanyJobs SET taken = 1 WHERE (job_id = {});", job->GetID());
		std::string insertEmployee = fmt::format("INSERT INTO CompanyEmployees(company_id,employee_id,job_id) VALUES({},{},{});", company->GetAccountID(), person->GetAccountID(), job->GetID());
		db->Execute(removeRequest);
		db->Execute(removeJob);
		db->Execute(insertEmployee);
		company->AddEmployee(person);
		company->RemoveJob(job);
		person->SetJob(job);
	}

	void MainLayer::DenyJobRequest(const std::string& id, Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();
		std::string removeRequest = fmt::format("DELETE FROM CompanyJobRequest WHERE (job_id = {} AND requester_id = {});",id,person->GetAccountID());
		db->Execute(removeRequest);
	}

	void MainLayer::UpdatePerson(Ref<Person> person, const std::string& imagepath, const std::string& firstname, const std::string& lastname, const std::string& email, const std::vector<std::string>& skills)
	{
		auto db = Application::Get()->GetDatabase();
		{

			std::string update = fmt::format("UPDATE Person SET first_name = '{}',last_name = '{}' WHERE account_id = {}", firstname, lastname,person->GetAccountID());
			db->Execute(update);
		}
		{
			std::string update = fmt::format("UPDATE Account SET email = '{}',image_path = '{}' WHERE account_id = {}", email, imagepath,person->GetAccountID());
			db->Execute(update);
		}
		{
			std::string remove = fmt::format("DELETE FROM PersonSkills WHERE (person_id = {});", person->GetAccountID());
			db->Execute(remove);
		}
		{
			std::string insertSkill = "INSERT Into PersonSkills VALUES";

			for (auto& skill : skills) {
				insertSkill += fmt::format("({},'{}'),", person->GetAccountID(), skill);
			}
			if (skills.size() != 0) {
				insertSkill.pop_back();
				insertSkill.push_back(';');
				db->Execute(insertSkill);
			}
		}
		person->SetImagePath(imagepath);
		person->SetFirstName(firstname);
		person->SetLastName(lastname);
		person->SetEmail(email);
		person->SetSkills(skills);
	}

	void MainLayer::UpdateCompany(Ref<Company> company, const std::string& companyName, const std::string& imagepath, const std::string& phone, const std::string& email)
	{
		auto db = Application::Get()->GetDatabase();

		{
			std::string update = fmt::format("UPDATE Company SET company_name = '{}' WHERE account_id = {}", companyName,company->GetAccountID());
			db->Execute(update);
		}
		{
			std::string update = fmt::format("UPDATE Account SET phone_number = '{}',email = '{}',image_path = '{}' WHERE account_id = {}", phone, email,imagepath,company->GetAccountID());
			db->Execute(update);
		}
		company->SetCompanyName(companyName);
		company->SetImagePath(imagepath);
		company->SetPhoneNumber(phone);
		company->SetEmail(email);
	}

	std::vector<Ref<Person>> MainLayer::GetUserSuggestions(Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();

		std::string queryLikes = fmt::format(R"(
		SELECT account_id FROM Person Where account_id IN(
			SELECT DISTINCT liked_by
			FROM Like
			WHERE post_id IN (
			    SELECT post_id FROM Like WHERE liked_by = {}
			)
		) AND account_id != {} AND account_id NOT IN(SELECT following_id FROM Following WHERE (follower_id = {}));
		)", person->GetAccountID(), person->GetAccountID(),person->GetAccountID());

		auto results = db->ExecuteAndReturn(queryLikes);

		if (results.size() <= 5) {
			std::string query = fmt::format(R"(
				SELECT account_id FROM Person WHERE account_id != {} AND account_id NOT IN(SELECT following_id FROM Following WHERE (follower_id = {}))  ORDER BY RANDOM();
			)",person->GetAccountID(),person->GetAccountID());
			auto r = db->ExecuteAndReturn(query);
			results.insert(results.end(), r.begin(), r.end());
		}

		std::vector<Ref<Person>> suggestions;
		for (auto& userID : results)
		{
			suggestions.push_back(GetPerson(userID.value("account_id").toString().toStdString()));
		}
		return suggestions;
	}

	std::vector<Ref<Person>> MainLayer::GetUserRequests(Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();

		std::string queryRequests = fmt::format("SELECT requester_id FROM FollowRequest Where(person_id = {});", person->GetAccountID());

		auto results = db->ExecuteAndReturn(queryRequests);
		
		std::vector<Ref<Person>> requests;

		for (auto& userID : results) {
			requests.push_back(GetPerson(userID.value("requester_id").toString().toStdString()));
		}
		return requests;
	}

	std::vector<Ref<Person>> MainLayer::GetCompanyFollowers(Ref<Company> company)
	{
		auto db = Application::Get()->GetDatabase();

		std::string queryCompanyFollowers = fmt::format(R"(
        SELECT DISTINCT p.account_id
        FROM Following f
        JOIN Person p ON f.follower_id = p.account_id
        JOIN Company c ON f.following_id = c.account_id
        WHERE c.account_id = {}
		)", company->GetAccountID());

		auto results = db->ExecuteAndReturn(queryCompanyFollowers);
		std::vector<Ref<Person>> followers;

		for (auto& userID : results) {
			followers.push_back(GetPerson(userID.value("account_id").toString().toStdString()));
		}
		return followers;
	}

	std::vector<PersonJobs> MainLayer::GetUserJobs(Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();
		
		std::string queryUserJobs = fmt::format(R"(
			SELECT DISTINCT j.job_id
			FROM CompanyJobs j
			WHERE j.taken = 0;
		)", person->GetAccountID());

		auto result = db->ExecuteAndReturn(queryUserJobs);

		std::vector<PersonJobs> jobs;

		for (auto& jobID : result) {
			PersonJobs job;
			job.Job = GetJob(jobID.value("job_id").toString().toStdString());

			std::string queryUserJobs = fmt::format("SELECT * FROM CompanyJobRequest WHERE (job_id = {} AND requester_id = {});", job.Job->GetID(), person->GetAccountID());
			auto res = db->ExecuteAndReturn(queryUserJobs);

			assert(res.size() < 2);
			if (res.empty())
				job.JobState = PersonJobs::State::None;
			else
				job.JobState = PersonJobs::State::Pending;
			jobs.push_back(job);
		}
		return jobs;
	}

	std::vector<CompanyCreatedJob> MainLayer::GetCompanyCreatedJobs(Ref<Company> company)
	{
		auto db = Application::Get()->GetDatabase();

		std::string queryJobs = fmt::format("SELECT * FROM CompanyJobRequest cj JOIN CompanyJobs c ON c.job_id = cj.job_id WHERE (c.company_id = {})", company->GetAccountID());
		auto results = db->ExecuteAndReturn(queryJobs);

		std::vector<CompanyCreatedJob> jobs;

		for (auto& res : results) {
			auto job = GetJob(res.value("job_id").toString().toStdString());
			auto person = GetPerson(res.value("requester_id").toString().toStdString());
			if (auto ptr = std::find_if(jobs.begin(), jobs.end(), [job](auto&& ccj) {
				return ccj.Job == job;
				}); ptr != jobs.end()) 
			{
				ptr->PeopleRequested.push_back(person);
				continue;
			}

			CompanyCreatedJob ccj{};
			ccj.Job = job;
			ccj.PeopleRequested.push_back(person);
			jobs.push_back(ccj);
		}
	
		return jobs;
	}

	std::vector<Ref<Post>> MainLayer::GetSuggestedUserPostsFromFollowed(Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();

		std::string queryPostsFromFollowed = fmt::format(R"(
        SELECT 
		    post_id
		FROM 
		    (SELECT 
		         p.post_id,
		         p.time_sent
		     FROM 
		         Post p
		     JOIN 
		         Following f ON p.sender_id = f.following_id
		     WHERE 
		         f.follower_id = {}
		
		     UNION ALL
		
		     SELECT 
		         rp.post_id,
		         rp.repost_at AS time_sent
		     FROM 
		         Repost rp
		     JOIN 
		         Following f ON rp.account_id = f.following_id
		     WHERE 
		         f.follower_id = {}
			ORDER BY 
			time_sent DESC
		    ) AS posts_and_reposts;
		)", person->GetAccountID(),person->GetAccountID());

		auto results = db->ExecuteAndReturn(queryPostsFromFollowed);

		std::vector<Ref<Post>> posts;
		for (auto& postID : results) {
			posts.push_back(GetPost(postID.value("post_id").toString().toStdString()));
		}
		return posts;
	}

	std::vector<Ref<Post>> MainLayer::GetSuggestedUserPostsFromSameCompany(Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();
		
		std::string queryPostsFromCompany = fmt::format(R"(
		
		SELECT 
		    post_id
		FROM 
		    (SELECT 
		         p.post_id,
		         p.time_sent
		     FROM 
		         Post p
			JOIN CompanyEmployees ce ON p.sender_id = ce.employee_id
			JOIN CompanyEmployees userCe ON userCe.employee_id = {0}
			WHERE ce.company_id = userCe.company_id AND p.sender_id != {0}
		
		     UNION ALL
		
		     SELECT 
		         rp.post_id,
		         rp.repost_at AS time_sent
		     FROM 
		         Repost rp
		     JOIN CompanyEmployees ce ON rp.account_id = ce.employee_id
			JOIN CompanyEmployees userCe ON userCe.employee_id = {0}
			WHERE ce.company_id = userCe.company_id AND rp.account_id != {0}
			ORDER BY 
			time_sent DESC
		    ) AS posts_and_reposts;
		)", person->GetAccountID());

		auto results = db->ExecuteAndReturn(queryPostsFromCompany);

		std::vector<Ref<Post>> posts;
		for (auto& postID : results) {
			posts.push_back(GetPost(postID.value("post_id").toString().toStdString()));
		}

		return posts;
	}

	std::vector<Ref<Post>> MainLayer::GetSuggestedUserPostsFromRandom(Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();
		
		std::string queryPostsFromRandom = fmt::format(R"(
        SELECT p.post_id
        FROM Post p
        WHERE p.sender_id NOT IN (
            SELECT f.following_id
            FROM Following f
            WHERE f.follower_id = {}
        )
        AND p.sender_id NOT IN (
            SELECT ce.employee_id
            FROM CompanyEmployees ce
            JOIN CompanyEmployees userCe ON userCe.employee_id = {}
            WHERE ce.company_id = userCe.company_id
        ) AND p.sender_id != {}
        ORDER BY RANDOM()
		)", person->GetAccountID(), person->GetAccountID() , person->GetAccountID());

		auto results = db->ExecuteAndReturn(queryPostsFromRandom);

		std::vector<Ref<Post>> posts;
		for (auto& postID : results) {
			posts.push_back(GetPost(postID.value("post_id").toString().toStdString()));
		}
		return posts;
	}

	SuggestedUserPosts MainLayer::GetSuggestedUserPosts(Ref<Person> person)
	{
		SuggestedUserPosts posts{};

		posts.FollowingPosts = GetSuggestedUserPostsFromFollowed(person);
		posts.SuggestedPosts = GetSuggestedUserPostsFromSameCompany(person);
		posts.RandomPosts = GetSuggestedUserPostsFromRandom(person);
		return posts;
	}

	std::vector<Ref<Post>> MainLayer::GetSuggestedCompanyPostsFromFollowed(Ref<Company> company)
	{
		auto db = Application::Get()->GetDatabase();

		std::string queryPostsFromFollowed = fmt::format(R"(
        SELECT 
		    post_id
		FROM 
		    (SELECT 
		         p.post_id,
		         p.time_sent
		     FROM 
		         Post p
		     JOIN 
		         Following f ON p.sender_id = f.following_id
		     WHERE 
		         f.follower_id = {}
		
		     UNION ALL
		
		     SELECT 
		         rp.post_id,
		         rp.repost_at AS time_sent
		     FROM 
		         Repost rp
		     JOIN 
		         Following f ON rp.account_id = f.following_id
		     WHERE 
		         f.follower_id = {}
			ORDER BY 
			time_sent DESC
		    ) AS posts_and_reposts;
		)", company->GetAccountID(), company->GetAccountID());

		auto results = db->ExecuteAndReturn(queryPostsFromFollowed);

		std::vector<Ref<Post>> posts;
		for (auto& postID : results) {
			posts.push_back(GetPost(postID.value("post_id").toString().toStdString()));
		}
		return posts;
	}

	const std::unordered_map<std::string, std::string>& MainLayer::GetIDToUsername()
	{
		return m_IDToUsername;
	}


#define TEST 0

	MainLayer::MainLayer()
	{
		s_Instance = this;
	}

	void MainLayer::OnAttach() {
		#if TEST==0
		QMainWindow* window = Window::GetMainWindow()->GetNativeHandle();

		m_WindowCentralWidget = new QWidget(window);

		window->setCentralWidget(m_WindowCentralWidget);

		window->resize(1280, 720);

		window->setWindowTitle(QApplication::translate("window", "LinkedOut", nullptr));

		m_WindowCentralLayout = new QVBoxLayout;
		m_LayersLayout = new QVBoxLayout;
		m_LayersWidget = new QWidget;
		m_LayersLayout->setContentsMargins(0, 0, 0, 0);
		
		{
			auto db = Application::Get()->GetDatabase();
			std::string cmd = "SELECT user_name,account_id FROM Account;";
			auto results = db->ExecuteAndReturn(cmd);

			for (auto& rec : results) {
				std::string id = rec.value("account_id").toString().toStdString();
				std::string username = rec.value("user_name").toString().toStdString();
				m_IDToUsername[id] = username;
			}
		}

		m_MessageLayer = new MessageLayer(this);

		SwitchToSplash();
//#define person
//#ifdef company
//		LoginCompany("2");
//		SwitchToNavigation(); 
//#endif
//#ifdef person
//		LoginPerson("1");
//		SwitchToNavigation();
//#endif
			
		m_LayersWidget->setLayout(m_LayersLayout);

		m_WindowCentralLayout->addWidget(m_LayersWidget);

		m_WindowCentralLayout->setContentsMargins(0, 0, 0, 0);

		m_WindowCentralWidget->setLayout(m_WindowCentralLayout);

		QMetaObject::connectSlotsByName(window);

		#else
		
		#endif	
	}

	void MainLayer::OnDetach(){
	}

	void MainLayer::OnUpdate(){
#if TEST ==0 
		m_CurrentLayer->OnUpdate();
		m_MessageLayer->OnUpdate();
#else
		treeWidget->show();

#endif
	}
	
	void MainLayer::SetCurrentLayer(Layer* layer) {
		if (m_CurrentLayer) {
			//m_WindowCentralLayout->removeWidget((QWidget*)m_CurrentLayer->GetMainFrame());
			m_CurrentLayer->Hide();
		}
		//m_WindowCentralLayout->addWidget((QWidget*)layer->GetMainFrame());
		m_CurrentLayer = layer;
		layer->Show();
		m_MessageLayer->Show();
	}

	void MainLayer::SwitchToSignup()
	{
		delete m_CurrentLayer;
		m_HomeLayer = {};
		m_JobPanel = {};
		m_NetworkPanel = {};
		m_MessagePanel = {};
		m_ProfilePanel = {}; 
		m_ViewProfilePanel = {};
		m_CurrentLayer = new SignupLayer(this);
		m_MessageLayer->Show();
	}

	void MainLayer::SwitchToLogin()
	{
		delete m_CurrentLayer;
		m_HomeLayer = {};
		m_JobPanel = {};
		m_NetworkPanel = {};
		m_MessagePanel = {};
		m_ProfilePanel = {}; 
		m_ViewProfilePanel = {};
		m_CurrentLayer = new LoginLayer(this);
		m_MessageLayer->Show();
	}

	void MainLayer::SwitchToSplash()
	{
		delete m_CurrentLayer;
		m_HomeLayer = {};
		m_JobPanel = {};
		m_NetworkPanel = {};
		m_MessagePanel = {};
		m_ProfilePanel = {};
		m_ViewProfilePanel = {};
		m_CurrentLayer = new SplashLayer(this);
		m_MessageLayer->Show();
	}

	void MainLayer::SwitchToNavigation()
	{
		delete m_CurrentLayer;
		m_HomeLayer = CreateRef<HomeLayer>(nullptr);
		m_JobPanel = CreateRef<JobPanel>(nullptr);
		m_NetworkPanel = CreateRef<NetworkPanel>(nullptr);
		m_MessagePanel = CreateRef<MessagePanel>(nullptr);
		m_ProfilePanel = CreateRef<ProfilePanel>(nullptr);
		m_ViewProfilePanel = CreateRef<OtherProfilePanel>(nullptr);

		{
			std::string buttonOnStyle =
				R"(
				QLabel{
					border:none;
					border-bottom:2px solid #1db82f;
					color: #1db82f;
				}
				QPushButton{
					border:none;
				}	
				)";

			std::string buttonOffStyle =
				R"(
				
				QPushButton{
					border:none;
				}					

				)";

			std::string profileOpenStyle =
				R"(
				
				QPushButton{
					border:none;
				}
				)";

			NavigationMenuSpecification spec{};
			spec.ButtonMinWidth = 20;
			spec.ButtonHeight = 40;
			spec.Buttons["Home"] = { LoadIcon("Resources/NavigationHomeButton.png"), m_HomeLayer };
			spec.Buttons["My Network"] = { LoadIcon("Resources/NavigationQueueIcon.png"), m_NetworkPanel };
			spec.Buttons["Jobs"] = { LoadIcon("Resources/NavigationJobsIcon.png"),m_JobPanel };
			spec.Buttons["Messaging"] = { LoadIcon("Resources/NavigationMessagingIcon.png"),m_MessagePanel };
			spec.ButtonOnStyle = buttonOnStyle;
			spec.ButtonOffStyle = buttonOffStyle;
			spec.ProfileOpenStyle = profileOpenStyle;
			spec.ProfileButton = { LoadIcon("Resources/NavigationProfileOpenIcon.png"),m_ProfilePanel };
			spec.ViewProfilePanel = m_ViewProfilePanel;
			auto ptr = new NavigationMenu(this, spec);
			m_CurrentLayer = ptr;
		}
		/*m_HomeLayer->SetContext(m_CurrentUser);
		m_JobPanel->SetContext(m_CurrentUser);
		m_NetworkPanel->SetContext(m_CurrentUser);
		m_MessagePanel->SetContext(m_CurrentUser);
		m_ProfilePanel->SetContext(m_CurrentUser);*/
		((NavigationMenu*)m_CurrentLayer)->SetActiveTab("Home");
		m_MessageLayer->Show();
	}

}