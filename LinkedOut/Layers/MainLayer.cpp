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


	void MainLayer::SwitchToLogin(bool cleanAll) {
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
	}

	void MainLayer::SwitchToHome()
	{
		SetCurrentLayer(m_HomeLayer);
	}

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
			std::string queryPosts = fmt::format("SELECT post_id FROM Post Where (sender_id = {});", id);
			auto posts = db->ExecuteAndReturn(queryPosts);

			std::vector<Ref<Post>> userPosts;

			for (auto& post : posts) {
				userPosts.push_back(GetPost(post.value("post_id").toString().toStdString()));
			}
			person->SetPosts(userPosts);
		}

		//TODO: implement DMs.

		return person;
	}

	Ref<Company> MainLayer::QueryCompany(const std::string& id,Ref<Company> company)
	{
		auto db = Application::Get()->GetDatabase();
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

		std::string company_name = companyDet.value("company_name").toString().toStdString();
		company->SetCompanyName(company_name);


		std::string queryJobs = fmt::format("SELECT job_id FROM CompanyJobs WHERE (company_id = {});", id);
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
		return company;
	}

	Ref<Job> MainLayer::QueryJob(const std::string& id,Ref<Job> job)
	{
		auto db = Application::Get()->GetDatabase();
		
		std::string queryJob = fmt::format("SELECT * FROM Job WHERE (job_id = {});", id);
		auto _rows = db->ExecuteAndReturn(queryJob);

		assert(_rows.size() == 1);
		auto& row = _rows[0];

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
		int repost_counter = postDet.value("repost_counter").toInt();
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

	void MainLayer::Follow(const std::string& id)
	{
		if (m_CurrentUser->GetAccountID() == id)
			return;
		m_CurrentUser->Follow(id);

		auto db = Application::Get()->GetDatabase();
		std::string insertFollowing = fmt::format("INSERT INTO FOLLOWING(follower_id,following_id) VALUES ({},{});", m_CurrentUser->GetAccountID(), id);
		db->Execute(insertFollowing);
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
		);
		)", person->GetAccountID());

		auto results = db->ExecuteAndReturn(queryLikes);

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

		std::string queryRequests = fmt::format("SELECT requester_id FROM PersonFollowRequests Where(person_id = {});", person->GetAccountID());

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

	std::vector<Ref<Job>> MainLayer::GetUserJobs(Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();
		
		std::string queryUserJobs = fmt::format(R"(
			SELECT DISTINCT j.job_id
			FROM Job j
			JOIN JobSkills js ON j.job_id = js.job_id
			JOIN PersonSkills ps ON js.skill_tag = ps.skill_tag
			WHERE ps.person_id = {}
		)", person->GetAccountID());

		auto result = db->ExecuteAndReturn(queryUserJobs);

		std::vector<Ref<Job>> jobs;

		for (auto& jobID : result) {
			jobs.push_back(GetJob(jobID.value("job_id").toString().toStdString()));
		}
		return jobs;
	}

	std::vector<Ref<Post>> MainLayer::GetSuggestedUserPostsFromFollowed(Ref<Person> person)
	{
		auto db = Application::Get()->GetDatabase();

		std::string queryPostsFromFollowed = fmt::format(R"(
        SELECT p.post_id
        FROM Post p
        JOIN Following f ON p.sender_id = f.following_id
        WHERE f.follower_id = {} AND p.post_id != {}
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
        SELECT p.post_id
        FROM Post p
        JOIN CompanyEmployees ce ON p.sender_id = ce.employee_id
        JOIN CompanyEmployees userCe ON userCe.employee_id = {}
        WHERE ce.company_id = userCe.company_id AND p.sender_id != {}
		)", person->GetAccountID(),person->GetAccountID());

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
        )
        ORDER BY RANDOM()
		)", person->GetAccountID(), person->GetAccountID());

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

	void MainLayer::SwitchToSignup(bool cleanAll) {

		SetCurrentLayer(m_SignupLayer);

		m_LastCaptchaNumber= Random::GenerateUInt32(1000, 10000);

		m_SignupLayer->m_CaptchaLabel->setText(QString("Captcha %1").arg(m_LastCaptchaNumber));
		if (cleanAll)
			m_SignupLayer->CleanAllInputs();
		else
			m_SignupLayer->CleanupInputs();
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
		
		
		m_SplashLayer = new SplashLayer(this);
		m_SplashLayer->Hide();
		m_SignupLayer = new SignupLayer(this);
		m_SignupLayer->Hide();
		m_LoginLayer = new LoginLayer(this);
		m_LoginLayer->Hide();
		m_MessageLayer = new MessageLayer(this);
		m_UserInformationLayer = new UserInformationLayer(this);
		m_UserInformationLayer->Hide();
		m_HomeLayer = new HomeLayer(this);
		m_HomeLayer->Hide();

		/*{

			Ref<Comment> c = CreateRef<Comment>();


			c->SetPostID("1");
			c->SetContentText("HelloAAdsadasdasfasfasfasfasfsafaasfsafasf");
			c->SetTimeSent(Time("21", "1", "1403", "19", "06", "00"));
			c->SetCommenterName("D");
			c->SetCommentID("2");

			Ref<Post> p = CreateRef<Post>();
			p->SetPosterName("A");
			p->SetPostID("1");
			p->SetContentText("HelloAAdsadasdasfasfasfasfasfsafaasfsafasf");
			p->SetRepostCounter(20);
			p->SetTimeSent(Time("21", "1", "1403", "19", "03", "00"));
			p->SetComments({ c });
			m_HomeLayer->AddPost(p);
			m_HomeLayer->ShowMore(10);
		}*/

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
			spec.Buttons["Home"] = { LoadIcon("Resources/NavigationHomeButton.png"), []() {} };
			spec.Buttons["My Network"] = { LoadIcon("Resources/NavigationQueueIcon.png"), []() {} };
			spec.Buttons["Jobs"] = { LoadIcon("Resources/NavigationJobsIcon.png"),[]() {} };
			spec.Buttons["Messaging"] = { LoadIcon("Resources/NavigationMessagingIcon.png"),[]() {} };
			spec.ButtonOnStyle = buttonOnStyle;
			spec.ButtonOffStyle = buttonOffStyle;
			spec.ProfileOpenStyle = profileOpenStyle;
			m_NavigationMenu = new NavigationMenu(this, spec);
		}
		m_NavigationMenu->Hide();
		
		m_UserData = GetUserDataStoredLocally();

		/*SetCurrentLayer(m_SplashLayer);

		if (!m_UserData.IsValid()) {
			SetCurrentLayer(m_SplashLayer);
		}*/
		UserInternalData d;
		d.Username = "john_doe";
		d.Password = "password123";
		Login(d, false);

		SetCurrentLayer(m_HomeLayer);

		
		m_LayersWidget->setLayout(m_LayersLayout);

		m_WindowCentralLayout->addWidget(m_LayersWidget);
		
		m_WindowCentralLayout->setContentsMargins(0, 0, 0, 0);


		m_WindowCentralWidget->setLayout(m_WindowCentralLayout);
		
		QMetaObject::connectSlotsByName(window);

		#else
		
		#endif	
	}

	void MainLayer::OnDetach(){
		delete m_LoginLayer;
		delete m_SignupLayer;
		delete m_SplashLayer;
		delete m_WindowCentralWidget;
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
		m_NavigationMenu->Show();
	}

	bool MainLayer::UserExists(const std::string& username) {
		auto db = Application::Get()->GetDatabase();
		std::string statement = fmt::format("SELECT * FROM USER WHERE (USERNAME == '{}');", username);
		auto res = db->ExecuteAndReturn(statement);
		return !res.empty();

	}

	UserInternalData MainLayer::GetUserDataFromUsername(const std::string& userName) {
		return {};
	}

	UserData MainLayer::GetUserInformation(uint32_t userID)
	{
		auto db = Application::Get()->GetDatabase();

		UserData userData{};

		//Jobs
		{
			std::string statement = fmt::format("SELECT * FROM WANTEDUSERJOB WHERE (UNIQUE_USERID = '{}');",userID);

			auto res = db->ExecuteAndReturn(statement);

			for (auto& row : res) {
				userData.WantedJobPositions.push_back(row.field("JOB_TITLE").value().toString().toStdString());
			}

		}
		//Company
		{
			std::string statement = fmt::format("SELECT * FROM WANTEDCOMPANYJOB WHERE (UNIQUE_USERID = '{}');", userID);

			auto res = db->ExecuteAndReturn(statement);

			for (auto& row : res) {
				userData.WantedJobPositions.push_back(row.field("COMPANY_NAME").value().toString().toStdString());
			}

		}

		std::string statement = fmt::format("SELECT * FROM USER WHERE (UNIQUE_USERID = '{}');", userID);
		auto res = db->ExecuteAndReturn(statement);
		assert(res.size() == 1);

		auto& record = res[0];

		auto firstName = record.field("FIRSTNAME").value().toString().toStdString();
		auto lastName = record.field("LASTNAME").value().toString().toStdString();

		auto birthDate = record.field("BIRTHDATE").value().toString().toStdString();

		auto placeOfStudy = record.field("PLACEOFSTUDY").value().toString().toStdString();

		userData.FirstName = firstName;
		userData.LastName = lastName;
		userData.PlaceOfEducation = placeOfStudy;
		userData.BirthDate = birthDate;

		return userData;
	}

	uint32_t MainLayer::GetUniqueUserID(const std::string& userName)
	{
		auto db = Application::Get()->GetDatabase();



		std::string statement = fmt::format("SELECT * FROM USER WHERE (USERNAME = '{}');", userName);
		try {
			auto res = db->ExecuteAndReturn(statement);
			if (res.size() != 1)
				return -1;

			return res[0].field("UNIQUE_USERID").value().toUInt();
		}
		catch (...) {
			return -1;
		}
		return -1;

	}

	SignupErrorCodes MainLayer::Signup(const UserInternalData& userData) {
		auto db= Application::Get()->GetDatabase();
		
		{
			std::string statement = fmt::format("SELECT * FROM USER WHERE (USERNAME = '{}');", userData.Username);
			auto res = db->ExecuteAndReturn(statement);
			if (!res.empty())
				return SignupErrorCodes::UserExists;
		}

		std::string passwordSalt = PasswordUtils::GenerateSalt();
		std::string passwordHash = PasswordUtils::HashPassword(userData.Password, passwordSalt);

		std::string statement = fmt::format("INSERT INTO USER(USERNAME,PASSWORD_HASH,PASSWORD_SALT) VALUES ('{}','{}','{}');", userData.Username, passwordHash,passwordSalt);

		try {
			db->Execute(statement);
			
		}
		catch (QueryException& exception) {
			return SignupErrorCodes::UserExists;
		}
		return SignupErrorCodes::None;
	}

	LoginErrorCodes MainLayer::Login(const UserInternalData& userData,bool storeLocally) {
		auto db = Application::Get()->GetDatabase();

		std::string queryValidateUser = fmt::format(R"(
			SELECT A.account_id, 
			       CASE 
			           WHEN P.account_id IS NOT NULL THEN 0 
			           WHEN C.account_id IS NOT NULL THEN 1
			           ELSE 2
			       END AS account_type
			FROM Account A
			LEFT JOIN Person P ON A.account_id = P.account_id
			LEFT JOIN Company C ON A.account_id = C.account_id
			WHERE A.user_name = '{}' AND A.password = '{}'
		)", userData.Username, userData.Password);

		auto res = db->ExecuteAndReturn(queryValidateUser);
		
		if (res.empty())
			return LoginErrorCodes_IncorrectUsername | LoginErrorCodes_IncorrectPassword;

		assert(res.size() == 1);

		std::string account_id = res[0].value("account_id").toString().toStdString();
		int account_type = res[0].value("account_type").toInt();

		switch (account_type)
		{
		case 0: m_CurrentUser = GetPerson(account_id); break;
		case 1: m_CurrentUser = GetCompany(account_id); break;
		default:
			return LoginErrorCodes_DBError;
		}

		if (storeLocally)
			StoreUserDataLocally(userData);

		return LoginErrorCodes_None;
	}

	template<typename _ReturnType,typename _Iterator>
	_ReturnType Join(_Iterator begin, _Iterator end, const _ReturnType& splitter) {

		_ReturnType result{};

		for (_Iterator current = begin; current != end; ++current) {
			result += *current;
			if (current + 1 != end)
				result += splitter;
		}

		return result;
	}


	UserInformationStoreErrorCodes MainLayer::StoreUserInformation(const UserData& userData)
	{


		auto db = Application::Get()->GetDatabase();

		try {
			//Delete old data
			{
				std::string jobDelete = fmt::format("DELETE FROM WANTEDUSERJOB WHERE UNIQUE_USERID = '{}'", m_UserData.UniqueUserID);
				db->Execute(jobDelete);
				std::string companyDelete = fmt::format("DELETE FROM WANTEDUSERCOMPANY WHERE UNIQUE_USERID = '{}'", m_UserData.UniqueUserID);
				db->Execute(companyDelete);
			}

			std::string statement = fmt::format("UPDATE USER SET FIRSTNAME = '{}',LASTNAME = '{}',BIRTHDATE = '{}',PLACEOFSTUDY = '{}' WHERE UNIQUE_USERID = '{}'", userData.FirstName, userData.LastName,
				userData.BirthDate, userData.PlaceOfEducation, m_UserData.UniqueUserID);

			db->Execute(statement);

			{
				std::string jobInsertCommand{};
				for (auto& job : userData.WantedJobPositions) {
					jobInsertCommand += fmt::format("INSERT INTO WANTEDUSERJOB(UNIQUE_USERID,JOB_TITLE) VALUES('{}','{}');", m_UserData.UniqueUserID, job);
				}
				if(!jobInsertCommand.empty())
					db->Execute(jobInsertCommand);
			}

			{
				std::string companyInsertCommand{};
				for (auto& company : userData.WantedCompanies) {
					companyInsertCommand += fmt::format("INSERT INTO WANTEDUSERCOMPANY(UNIQUE_USERID,COMPANY_NAME) VALUES('{}','{}');", m_UserData.UniqueUserID, company);
				}
				if(!companyInsertCommand.empty())
				db->Execute(companyInsertCommand);
			}
		}
		catch(std::exception& err) {
			return UserInformationStoreErrorCodes_DBError;
		}
	
		return UserInformationStoreErrorCodes_None;
	}

	
	

}